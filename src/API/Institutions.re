type media = {
  source: string,
  type_: string,
};

type institution = {
  consentStatus: Consents.consentStatus,
  id: string,
  name: string,
  fullName: string,
  media: list(media),
  features: list(string),
};

type institutions = list(institution);

module Decode = {
  let media = json =>
    Json.Decode.{
      source: json |> field("source", string),
      type_: json |> field("type", string),
    };

  let institution = json =>
    Json.Decode.{
      consentStatus: Consents.AwaitingAuthorization,
      id: json |> field("id", string),
      name: json |> field("name", string),
      fullName: json |> field("fullName", string),
      media: json |> field("media", list(media)),
      features:
        json
        |> field(
             "features",
             map(
               arr => arr->Belt.List.keepMap(x => x),
               list(optional(string)),
             ),
           ),
    };

  let institutions = json =>
    json |> Json.Decode.(field("institutions", institution->list));
};
module Request = {
  let institutionsRequest = authToken => {
    let url = "http://localhost:8080/api/institutions";
    Fetch.fetchWithInit(
      url,
      Fetch.RequestInit.make(
        ~method_=Get,
        ~headers=
          Fetch.HeadersInit.make({
            "Content-Type": "application/json",
            "x-auth-token": authToken,
          }),
        (),
      ),
    );
  };

  let authoriseRequest = (authToken, ~userUuid, ~institutionId) => {
    let url = "http://localhost:8080/api/institutions/authorize";
    let payload =
      Json.Encode.(
        [
          ("userUuid", userUuid->string),
          ("institutionId", institutionId->string),
          ("callback", "http://localhost:8080"->string),
        ]
        ->object_
      )
      ->Js.Json.stringify;
    Fetch.fetchWithInit(
      url,
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=Fetch.BodyInit.make(payload),
        ~headers=
          Fetch.HeadersInit.make({
            "Content-Type": "application/json",
            "x-auth-token": authToken,
          }),
        (),
      ),
    );
  };

  exception RetrieveTokenError(string);
  let getInstitutions = () =>
    Js.Promise.(
      Auth.getAuthToken()
      |> then_(institutionsRequest)
      |> then_(Fetch.Response.json)
      |> then_(json => json->Decode.institutions->resolve)
    );
};

let authorise = (~userUuid, ~institutionId) =>
  Js.Promise.(
    Auth.getAuthToken()
    |> then_(Request.authoriseRequest(~userUuid, ~institutionId))
    |> then_(Fetch.Response.json)
    |> then_(json =>
         Json.Decode.(field("authorisationUrl", string, json))->resolve
       )
  );

let get = () =>
  Js.Promise.(
    all2((Request.getInstitutions(), Consents.get()))
    |> then_(((institutions, consents)) =>
         List.map(
           (i: institution) => {
             ...i,
             consentStatus:
               switch (
                 List.find(
                   (consent: Consents.consent) =>
                     consent.institutionId == i.id,
                   consents,
                 )
               ) {
               | exception Not_found => Consents.AwaitingAuthorization
               | consent => consent.status
               },
           },
           institutions,
         )
         ->resolve
       )
  );
