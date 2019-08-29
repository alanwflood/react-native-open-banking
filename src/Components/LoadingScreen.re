open ReactNative;
open ReactNavigation;
open GlobalStyles;

[@react.component]
let make = (~countdown=3) => {
  let (count, setCount) = React.useState(() => countdown);

  /* Only show loading spinner after 3 seconds */
  React.useEffect0(() => {
    let intervalId =
      Js.Global.setInterval(() => setCount(count => count - 1), 1000);
    Some(() => Js.Global.clearInterval(intervalId));
  });

  <View style=styles##container>
    {count == 0 ? <ActivityIndicator /> : ReasonReact.null}
    <StatusBar barStyle=`default />
  </View>;
};
