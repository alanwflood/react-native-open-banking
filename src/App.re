open ReactNative;
open User.Login;

[@react.component]
let app = () => {
  let (auth, setAuth) = React.useState(() => Auth.LoggedOut);

  <Auth.Provider value=Auth.{auth: (auth, setAuth)}>
    <Text> auth->Auth.authToString->React.string </Text>
    <SignIn />
  </Auth.Provider>;
};
