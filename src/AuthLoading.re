open ReactNative;
open ReactNavigation;
open GlobalStyles;

[@react.component]
let make = (~navigation: Navigation.t, ~countdown=3) => {
  let (_, setAuth) = React.useContext(Auth.context).Auth.auth;
  let (count, setCount) = React.useState(() => countdown);

  /* Only show loading spinner after 3 seconds */
  React.useEffect0(() => {
    let intervalId =
      Js.Global.setInterval(() => setCount(count => count - 1), 1000);
    Some(() => Js.Global.clearInterval(intervalId));
  });

  React.useEffect1(
    () => {
      Auth.checkAuthWithRoute(~navigation, ~setAuth) |> ignore;
      Some(() => ());
    },
    [||],
  );

  <View style=styles##container>
    {count == 0 ? <ActivityIndicator /> : ReasonReact.null}
    <StatusBar barStyle=`default />
  </View>;
};
