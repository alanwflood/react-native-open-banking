open AppNavigator;

[@react.component]
let app = () => {
  let (auth, setAuth) = React.useState(() => Auth.LoggedOut);
  <Auth.Provider value=Auth.{auth: (auth, setAuth)}>
    <SumiAppContainer screenProps={"auth": auth} />
  </Auth.Provider>;
};
