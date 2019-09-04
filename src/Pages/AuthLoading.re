open ReactNavigation;

[@react.component]
let make = (~navigation: Navigation.t, ~countdown=3) => {
  let (_, setUser) = React.useContext(Auth.context).Auth.auth;

  React.useEffect0(() => {
    Auth.checkAuthWithRoute(~navigation, ~setUser) |> ignore;
    Some(() => ());
  });

  <LoadingScreen />;
};
