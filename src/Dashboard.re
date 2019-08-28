open ReactNative;
open ReactNavigation;

module Ionicons = {
  [@bs.module "@expo/vector-icons"] [@react.component]
  external make:
    (
      ~children: React.element=?,
      ~name: string=?,
      ~size: int=?,
      ~color: string=?,
      ~style: ReactNative.Style.t=?
    ) =>
    React.element =
    "Ionicons";
};

[@react.component]
let make = (~navigation) => {
  let (auth, _) = React.useContext(Auth.context).Auth.auth;
  let user = auth->Auth.currentUserOrRaise;

  <View style=GlobalStyles.styles##container>
    <Text> ("Welcome to the dashboard " ++ user.name)->React.string </Text>
    <Button onPress={_ => Auth.logOut(~navigation)} title="Sign Out" />
  </View>;
};
make
->NavigationOptions.setNavigationOptions(
    NavigationOptions.t(
      ~tabBarIcon=
        NavigationOptions.TabBarIcon.render(_props =>
          <Icons.AntDesign name="dashboard" size=22 />
        ),
      (),
    ),
  );
