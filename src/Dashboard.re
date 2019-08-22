open ReactNative;
open ReactNavigation;

let styles =
  Style.(
    StyleSheet.create({
      "container":
        style(
          ~flex=1.,
          ~justifyContent=`center,
          ~alignItems=`center,
          ~backgroundColor="#F5FCFF",
          (),
        ),
    })
  );

[@react.component]
let make = (~navigation) =>
  <View style=styles##container>
    <Text> "Welcome to the dashboard"->React.string </Text>
    <Button
      onPress={_ => navigation->Navigation.navigate("SignIn")}
      title="Sign Out"
    />
  </View>;
