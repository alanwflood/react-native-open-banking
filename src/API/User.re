open ReactNative;
open ReactNavigation;

module Login = {
  type user = {
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

  let errorMessageDecoder = message =>
    switch (message) {
    | "Submitted credentials are incorrect" => InvalidCredentials
    | "Email address should be valid" => InvalidEmail
    | "Network request failed" => NetworkFailure
    | _ => UnknownError(message)
    };

  /* JSON Decoders */
  let decodeUser = json =>
    Success(
      Json.Decode.{
        createdDate:
          json
          |> field("createdDate", map(Js.Date.fromFloat, Json.Decode.float)),
        name: json |> field("name", string),
        id: json |> field("id", int),
        uuid: json |> field("uuid", string),
        email: json |> field("email", string),
      },
    );

  let decodeError = json =>
    Fail(
      Json.Decode.field(
        "message",
        Json.Decode.map(errorMessageDecoder, Json.Decode.string),
        json,
      ),
    );

  /* Logging in will either return a user or an error */
  let decodeLogin = json =>
    json |> Json.Decode.either(decodeUser, decodeError);

  /* Fetching methods */
  let decodeFetchError = _error =>
    [%raw "_error.message"]->errorMessageDecoder->Fail;

  let loginRequest = (email, password) => {
    let url = "http://localhost:8080/api/authentications";
    let payload = Js.Dict.empty();
    Js.Dict.set(payload, "email", Js.Json.string(email));
    Js.Dict.set(payload, "password", Js.Json.string(password));
    Fetch.fetchWithInit(
      url,
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=
          Fetch.BodyInit.make(payload->Js.Json.object_->Js.Json.stringify),
        ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
        (),
      ),
    );
  };

  /* Setting x-auth-token in AsyncStorage */
  exception NoAuthToken(string);
  let getAuthToken = response =>
    switch (
      response->Fetch.Response.headers |> Fetch.Headers.get("x-auth-token")
    ) {
    | Some(token) => token
    | None => raise(NoAuthToken("No auth token found in server response"))
    };

  let storeLoginDetails = (~user, ~authToken) =>
    Js.Promise.resolve(
      AsyncStorage.multiSet([|("user", user), ("authToken", authToken)|]),
    )
    |> ignore;
};

/* Login Action */
let login = (~email, ~password) => {
  let authToken = ref("");
  Js.Promise.(
    Login.loginRequest(email, password)
    |> then_(response => {
         authToken := response->Login.getAuthToken;
         response->Fetch.Response.json;
       })
    |> then_(json => {
         let user = json->Login.decodeLogin;
         Login.storeLoginDetails(
           ~user=Js.Json.stringify(json),
           ~authToken=authToken^,
         );
         user->resolve;
       })
    |> catch(error => error->Login.decodeFetchError->resolve)
  );
};
