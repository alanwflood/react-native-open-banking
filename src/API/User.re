open ReactNative;

module Login = {
  /* Login data Types */
  type user = {
    createdDate: Js.Date.t,
    name: string,
    id: int,
    uuid: string,
    email: string,
  };

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
  let setAuthToken = response =>
    switch (
      response->Fetch.Response.headers |> Fetch.Headers.get("x-auth-token")
    ) {
    | Some(token) =>
      Js.Promise.resolve(AsyncStorage.setItem("authToken", token)) |> ignore
    | None => raise(NoAuthToken("No auth token found in server response"))
    };

  /* Login Action */
  let login = (~email, ~password) =>
    Js.Promise.(
      loginRequest(email, password)
      |> then_(response => {
           response->setAuthToken;
           response->Fetch.Response.json;
         })
      |> then_(json => json->decodeLogin->resolve)
      |> catch(error => error->decodeFetchError->resolve)
    );
};
