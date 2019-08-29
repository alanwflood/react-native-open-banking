open ReactNative;
open ReactNavigation;

type token = string;

type authType =
  | LoggedIn(User.Login.user)
  | LoggedOut;

exception RetrieveUserError(string);
let currentUser = auth =>
  switch (auth) {
  | LoggedIn(user) => user
  | LoggedOut => raise(RetrieveUserError("user not found"))
  };

let authToString = thing =>
  switch (thing) {
  | LoggedIn(_user) => "Logged in"
  | LoggedOut => "Logged out"
  };

let isLoggedIn = auth =>
  switch (auth) {
  | LoggedIn(_user) => true
  | LoggedOut => false
  };

type authContext = {
  auth: (authType, (authType => authType) => unit),
  token: (token, (token => token) => unit),
};

let currentUserOrRaise = auth =>
  switch (auth) {
  | LoggedIn(user) => user
  | LoggedOut => raise(RetrieveUserError("Current User missing"))
  };

exception RetrieveUserError(string);
let getCurrentUser = (~setAuth) =>
  Js.Promise.(
    AsyncStorage.getItem("user")
    |> then_(json =>
         switch (json->Js.Null.toOption) {
         | None =>
           "Error retrieving user from async storage"->RetrieveUserError->raise
         | Some(json) =>
           json->Json.parseOrRaise->User.Login.decodeUser->resolve
         }
       )
    |> then_(user =>
         switch (user) {
         | User.Login.Success(user) => setAuth(_ => LoggedIn(user))->resolve
         | User.Login.Fail(_error) => setAuth(_ => LoggedOut)->resolve
         }
       )
    |> catch(_error => setAuth(_ => LoggedOut)->resolve)
  );

let getAuthToken = (~navigation) =>
  Js.Promise.(
    AsyncStorage.getItem("authToken")
    |> then_(nullableToken => {
         Js.log2("CHECKING_AUTH_TOKEN", nullableToken);
         resolve(
           switch (Js.Null.toOption(nullableToken)) {
           | None =>
             navigation->Navigation.navigate("SignIn");
             "";
           | Some(authToken) => authToken
           },
         );
       })
  );

/* If these two promises succeed it means we're logged in with a valid user */
let checkAuthWithRoute = (~navigation: Navigation.t, ~setAuth, ~setToken) =>
  Js.Promise.(
    all([|
      getCurrentUser(~setAuth),
      getAuthToken(~navigation)
      |> Js.Promise.then_(token => setToken(_ => token)->resolve),
    |])
    |> then_(_ => resolve(navigation->Navigation.navigate("App")))
  );

let logOut = (~navigation: Navigation.t) =>
  Js.Promise.(
    AsyncStorage.clear()
    |> then_(_result => navigation->Navigation.navigate("SignIn")->resolve)
    |> ignore
  );

let context =
  React.createContext({auth: (LoggedOut, _ => ()), token: ("", _ => ())});

module Provider = {
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };
  let make = React.Context.provider(context);
};
