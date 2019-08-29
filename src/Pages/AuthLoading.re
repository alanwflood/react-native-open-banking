open ReactNavigation;

[@react.component]
let make = (~navigation: Navigation.t, ~countdown=3) => {
  let (_, setAuth) = React.useContext(Auth.context).Auth.auth;
  let (_, setToken) = React.useContext(Auth.context).Auth.token;

  React.useEffect1(
    () => {
      Auth.checkAuthWithRoute(~navigation, ~setAuth, ~setToken) |> ignore;
      Some(() => ());
    },
    [||],
  );

  <LoadingScreen />;
};
