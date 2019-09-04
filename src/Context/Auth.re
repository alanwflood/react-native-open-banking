open ReactNative;
open ReactNavigation;
open User.Login;

type token = string;

type authType =
  | LoggedIn(User.Login.user)
  | LoggedOut;

exception RetrieveUserError(string);

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

let currentUserOrRaise = auth =>
  switch (auth) {
  | LoggedIn(user) => user
  | LoggedOut => raise(RetrieveUserError("Current User missing"))
  };

exception RetrieveUserError(string);
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
           Js.log2("user in storage:", user);
           user->resolve;
         }
       )
  );

exception RetrieveTokenError(string);
let getAuthToken = () =>
  Js.Promise.(
    getCurrentUser()
    |> then_(user => user->currentUserOrRaise.token->resolve)
    |> catch(_err =>
         "Could not retrieve token for current user"->RetrieveTokenError->raise
       )
  );

let logOut = (~navigation: Navigation.t) =>
  Js.Promise.(
    AsyncStorage.clear()
    |> then_(_result => navigation->Navigation.navigate("SignIn")->resolve)
    |> ignore
  );

/* If these two promises succeed it means we're logged in with a valid user */
let checkAuthWithRoute = (~navigation: Navigation.t, ~setUser) =>
  Js.Promise.(
    getCurrentUser()
    |> then_(user => setUser(_ => user)->resolve)
    |> then_(_ => resolve(navigation->Navigation.navigate("App")))
    |> catch(_err => logOut(~navigation)->resolve)
  );

type authContext = {auth: (authType, (authType => authType) => unit)};

let context = React.createContext({auth: (LoggedOut, _ => ())});

module Provider = {
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };
  let make = React.Context.provider(context);
};
