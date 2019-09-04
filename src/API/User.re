open ReactNative;

module Login = {
  type user = {
    token: string,
    createdDate: Js.Date.t,
    name: string,
    id: int,
    uuid: string,
    email: string,
  };

  /* Login data Types */
  type error =
    | NetworkFailure
    | InvalidCredentials
    | InvalidEmail
    | RequiredEmail
    | RequiredPassword
    | UnknownError(string);

  type loginStatus =
    | Success(user)
    | Fail(error);

  /* JSON Decoders */
  module Decode = {
    let stringToError = message =>
      switch (message) {
      | "Submitted credentials are incorrect" => InvalidCredentials
      | "Email address should be valid" => InvalidEmail
      | "Network request failed" => NetworkFailure
      | _ => UnknownError(message)
      };

    let user = json =>
      Success(
        Json.Decode.{
          /* Token is not found in the initial response but will be stored later on in async-storage */
          token:
            Belt.Option.getWithDefault(
              json |> optional(field("token", string)),
              "",
            ),
          createdDate:
            json
            |> field(
                 "createdDate",
                 map(Js.Date.fromFloat, Json.Decode.float),
               ),
          name: json |> field("name", string),
          id: json |> field("id", int),
          uuid: json |> field("uuid", string),
          email: json |> field("email", string),
        },
      );

    let error = json =>
      Fail(
        Json.Decode.field(
          "message",
          Json.Decode.map(stringToError, Json.Decode.string),
          json,
        ),
      );

    /* Logging in will either return a user or an error */
    let login = json => json |> Json.Decode.either(user, error);

    /* Convert server error message to an error type */
    let fetchError = _error => [%raw "_error.message"]->stringToError->Fail;
  };

  module Request = {
    let login = (email, password) => {
      let url = "http://localhost:8080/api/authentications";
      let payload =
        Json.Encode.(
          [("email", email->string), ("password", password->string)]
          ->object_
        )
        ->Js.Json.stringify;
      Fetch.fetchWithInit(
        url,
        Fetch.RequestInit.make(
          ~method_=Post,
          ~body=Fetch.BodyInit.make(payload),
          ~headers=
            Fetch.HeadersInit.make({"Content-Type": "application/json"}),
          (),
        ),
      );
    };
  };

  /* Getting x-auth-token from response */
  exception NoAuthToken(string);
  let getAuthToken = response =>
    switch (
      response->Fetch.Response.headers |> Fetch.Headers.get("x-auth-token")
    ) {
    | Some(token) => token
    | None => raise(NoAuthToken("No auth token found in server response"))
    };

  module Encode = {
    let user = user =>
      Json.Encode.(
        [
          ("token", user.token->string),
          (
            "createdDate",
            user.createdDate->Js.Date.getTime->Json.Encode.float,
          ),
          ("name", user.name->string),
          ("id", user.id->int),
          ("uuid", user.uuid->string),
          ("email", user.email->string),
        ]
        ->object_
      );
  };
};

/* Login Action */
let login = (~email, ~password) => {
  let authToken = ref("");
  Js.Promise.(
    Login.Request.login(email, password)
    |> then_(response => {
         authToken := response->Login.getAuthToken;
         response->Fetch.Response.json;
       })
    |> then_(json => {
         let user = json->Login.Decode.login;
         switch (user) {
         | Success(userData) =>
           Login.Encode.user({...userData, token: authToken^})->Json.stringify
           |> AsyncStorage.setItem("user")
           |> ignore
         | Fail(error) => error |> ignore
         };
         user->resolve;
       })
    |> catch(error => error->Login.Decode.fetchError->resolve)
  );
};
