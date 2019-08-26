open ReactNative;
open AppNavigator;

[@react.component]
let app = () => {
  let (auth, setAuth) = React.useState(() => Auth.LoggedOut);
  let (token, setToken) = React.useState(() => "");

  <Auth.Provider
    value=Auth.{auth: (auth, setAuth), token: (token, setToken)}>
    <Text style=GlobalStyles.styles##headingText>
      auth->Auth.authToString->React.string
    </Text>
    <SumiAppContainer screenProps={"token": token, "auth": auth} />
  </Auth.Provider>;
};
