type consentStatus =
  | AwaitingAuthorization
  | Authorized
  | Rejected
  | Revoked
  | Failed
  | Expired
  | Unknown;

type consent = {
  institutionId: string,
  status: consentStatus,
};

let stringToConsentStatus = status =>
  switch (status) {
  | "AWAITING_AUTHORIZATION" => AwaitingAuthorization
  | "AUTHORIZED" => Authorized
  | "REJECTED" => Rejected
  | "REVOKED" => Revoked
  | "FAILED" => Failed
  | "EXPIRED" => Expired
  | _ => Unknown
  };

type consentAccount = {
  id: string,
  institutionId: string,
  type_: string,
  balance: float,
  currency: string,
};

type ammendResponse = {
  consents: list(consent),
  accounts: list(consentAccount),
};

module Decoder = {
  let consent = json =>
    Json.Decode.{
      institutionId: json |> field("institutionId", string),
      status: json |> field("status", map(stringToConsentStatus, string)),
    };

  let account = json =>
    Json.Decode.{
      id: json |> field("id", string),
      institutionId: json |> field("institutionId", string),
      type_: json |> field("type", string),
      balance: json |> field("balance", Json.Decode.float),
      currency: json |> field("currency", string),
    };

  let ammendResponse = json =>
    Json.Decode.{
      consents: json |> field("consents", consent->list),
      accounts: json |> field("accounts", account->list),
    };
};

module Request = {
  let postConsentAmmend =
      (authToken, ~userUuid, ~institutionId, ~consentToken) => {
    let url = "http://localhost:8080/api/consent/";
    let payload =
      Json.Encode.(
        [
          ("userUuid", userUuid->string),
          ("institutionId", institutionId->string),
          ("consentToken", consentToken->string),
          ("preselection", (institutionId != "bbva-sandbox")->bool),
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

  let getConsents = authToken => {
    let url = "http://localhost:8080/api/consents/";
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
};

exception AmmendRequestError(string);
let ammend = (~userUuid, ~institutionId, ~consentToken) => {
  let request =
    Request.postConsentAmmend(~userUuid, ~institutionId, ~consentToken);
  Js.Promise.(
    Auth.getAuthToken()
    |> then_(request)
    |> then_(Fetch.Response.json)
    |> then_(json => {
         Js.log(json);
         json->Decoder.ammendResponse->resolve;
       })
    |> catch(err => {
         Js.log(err);
         AmmendRequestError(
           "Failed to ammend consent for institution" ++ institutionId,
         )
         ->reject;
       })
  );
};

let get = () =>
  Js.Promise.(
    Auth.getAuthToken()
    |> then_(Request.getConsents)
    |> then_(Fetch.Response.json)
    |> then_(json =>
         Json.Decode.(json |> field("consents", Decoder.consent->list))
         ->resolve
       )
  );
