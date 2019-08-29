type media = {
  source: string,
  type_: string,
};

type institution = {
  id: string,
  name: string,
  fullName: string,
  media: list(media),
  features: list(string),
};

type institutions = array(institution);

let decodeMedia = json =>
  Json.Decode.{
    source: json |> field("source", string),
    type_: json |> field("type", string),
  };

let decodeInstitution = json =>
  Json.Decode.{
    id: json |> field("id", string),
    name: json |> field("name", string),
    fullName: json |> field("fullName", string),
    media: json |> field("media", list(decodeMedia)),
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

let decodeInstitutions = json =>
  json
  |> Json.Decode.field("institutions", Json.Decode.array(decodeInstitution));

let institutionsRequest = (~authToken) => {
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

exception RetrieveTokenError(string);
let getList = () =>
  Js.Promise.(
    ReactNative.AsyncStorage.getItem("authToken")
    |> then_(nullableToken =>
         switch (Js.Null.toOption(nullableToken)) {
         | None => reject(RetrieveTokenError("Token not found"))
         | Some(token) => token->resolve
         }
       )
    |> then_(authToken => institutionsRequest(~authToken))
    |> then_(Fetch.Response.json)
    |> then_(json => json->decodeInstitutions->resolve)
  );

let authoriseRequest = (~authToken, ~userUuid, ~institutionId) => {
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
  Js.log(payload);
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

let authorise = (~authToken, ~userUuid, ~institutionId) =>
  Js.Promise.(
    authoriseRequest(~authToken, ~userUuid, ~institutionId)
    |> then_(Fetch.Response.json)
    |> then_(json =>
         Json.Decode.(field("authorisationUrl", string, json))->resolve
       )
  );
