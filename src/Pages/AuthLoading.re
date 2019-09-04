open ReactNavigation;

[@react.component]
let make = (~navigation: Navigation.t, ~countdown=3) => {
  let (_user, setUser) = React.useContext(Auth.context).Auth.auth;

  let checkAuthWithRoute = () =>
    Js.Promise.(
      Auth.getCurrentUser()
      |> then_(user => setUser(_ => user)->resolve)
      |> then_(_ => resolve(navigation->Navigation.navigate("App")))
      |> catch(_err => Auth.logOut(~navigation)->resolve)
    )
    |> ignore;

  React.useEffect0(() => {
    checkAuthWithRoute();
    Some(() => ());
  });

  <LoadingScreen />;
};
