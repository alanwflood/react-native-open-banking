open AppNavigator;

[@react.component]
let app = () => {
  let (auth, setAuth) = React.useState(() => Auth.LoggedOut);
  let (token, setToken) = React.useState(() => "");

  <Auth.Provider
    value=Auth.{auth: (auth, setAuth), token: (token, setToken)}>
    <SumiAppContainer screenProps={"token": token, "auth": auth} />
  </Auth.Provider>;
};
