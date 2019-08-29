open ReactNative;
open ReactNavigation;

[@react.component]
let make = (~navigation) => {
  let (auth, _) = React.useContext(Auth.context).Auth.auth;
  let user = auth->Auth.currentUserOrRaise;

  let url = Expo.Linking.makeUrl("accounts", ());

  let s = "http://localhost:8080/?consent=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzUxMiJ9.eyJJTlNUSVRVVElPTiI6ImJidmEtc2FuZGJveCIsIkNPTlNFTlQiOiIwZDc0NGJhZC04ZDczLTQ5NDMtODEyYS1hYTI5ODJhODMwMDYiLCJBUFBMSUNBVElPTl9VU0VSX0lEIjoiYUBmLmNvbSIsIlVTRVIiOiI4OGM4ZWE2Yy0yNWEzLTRlY2ItYjEzNC1hNGIyM2RkZTJkZjcifQ.lGhpcwuFEkzu7YrpyoPh-Pretpmw1HVz0gU9NBJXA4xFgaqlAF8GDtemaE-gWM_m6_pPY3hWLPaYxZGelAvsfw&application-user-id=a%40f.com&user-uuid=88c8ea6c-25a3-4ecb-b134-a4b23dde2df7&institution=bbva-sandbox";
  Js.log(Utils.parseUrlParams(s));

  <View style=GlobalStyles.styles##container>
    <Text> ("Welcome to the dashboard " ++ user.name)->React.string </Text>
    <Button onPress={_ => Auth.logOut(~navigation)} title="Sign Out" />
    <Button
      onPress={
        _ => {
          Js.log(url);
          Linking.openURL(url)
          |> Js.Promise.catch(err => err->Js.log->Js.Promise.resolve)
          |> ignore;
        }
      }
      title="Test?"
    />
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
