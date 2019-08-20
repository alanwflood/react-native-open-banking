/* Login data Types */
type user = {
  createdDate: Js.Date.t,
  name: string,
  id: int,
  uuid: string,
  email: string,
};

type error = {message: string};

type loginStatus =
  | Success(user)
  | Fail(error);

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
  Fail(Json.Decode.{message: json |> field("message", string)});

/* Logging in will either return a user or an error */
let decodeLogin = json => json |> Json.Decode.either(decodeUser, decodeError);

/* Fetching methods */
let decodeFetchError = _error => {message: [%raw "_error.message"]}->Fail;

let loginRequest = (email, password) => {
  let url = "http://localhost:8080/api/authentications";
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "email", Js.Json.string(email));
  Js.Dict.set(payload, "password", Js.Json.string(password));
  Fetch.fetchWithInit(
    url,
    Fetch.RequestInit.make(
      ~method_=Post,
      ~body=Fetch.BodyInit.make(payload->Js.Json.object_->Js.Json.stringify),
      ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
      (),
    ),
  );
};

/* Login Action */
let login = (email, password) =>
  Js.Promise.(
    loginRequest(email, password)
    |> then_(Fetch.Response.json)
    |> then_(json => json->decodeLogin->resolve)
    |> catch(error => error->decodeFetchError->resolve)
  );
