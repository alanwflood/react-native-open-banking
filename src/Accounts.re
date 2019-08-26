open ReactNative;
open ReactNavigation;

[@react.component]
let make = () => {
  let (auth, _) = React.useContext(Auth.context).Auth.auth;
  let user = auth->Auth.currentUserOrRaise;

  <View style=GlobalStyles.styles##container>
    <Text> ("Welcome to the accounts page " ++ user.name)->React.string </Text>
  </View>;
};
