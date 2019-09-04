open ReactNative;
open ReactNavigation;
open User.Login;

type token = string;

type authorization =
  | LoggedIn(User.Login.user)
  | LoggedOut;

let mapUserLoginToAuth = loginStatus =>
  switch (loginStatus) {
  | Success(user) => LoggedIn(user)
  | Fail(_err) => LoggedOut
  };

let isLoggedIn = auth =>
  switch (auth) {
  | LoggedIn(_user) => true
  | LoggedOut => false
  };

exception RetrieveUserError(string);
let currentUserOrRaise = auth =>
  switch (auth) {
  | LoggedIn(user) => user
  | LoggedOut => raise(RetrieveUserError("Current User missing"))
  };

let getCurrentUser = () =>
  Js.Promise.(
    AsyncStorage.getItem("user")
    |> then_(json =>
         switch (json->Js.Null.toOption) {
         | None => LoggedOut->resolve
         | Some(json) =>
           let user =
             json
             ->Json.parseOrRaise
             ->User.Login.Decode.user
             ->mapUserLoginToAuth;
           user->resolve;
         }
       )
    |> catch(_err =>
         "Could not get user from storage"->RetrieveUserError->raise
       )
  );

let logOut = (~navigation: Navigation.t) =>
  Js.Promise.(
    AsyncStorage.clear()
    |> then_(_result => navigation->Navigation.navigate("SignIn")->resolve)
    |> ignore
  );

exception RetrieveTokenError(string);
let getAuthToken = () =>
  Js.Promise.(
    getCurrentUser() |> then_(user => user->currentUserOrRaise.token->resolve)
  );

type authContext = {
  auth: (authorization, (authorization => authorization) => unit),
};

let context = React.createContext({auth: (LoggedOut, _ => ())});

module Provider = {
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };
  let make = React.Context.provider(context);
};
