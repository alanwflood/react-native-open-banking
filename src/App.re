open AppNavigator;

[@react.component]
let app = () => {
  let (auth, setAuth) = React.useState(() => Auth.LoggedOut);
  let (institutions, setInstitutions) =
    React.useState(() => Institution.NotAsked);

  <Auth.Provider value=Auth.{auth: (auth, setAuth)}>
    <Institution.Provider
      value=Institution.{institutions: (institutions, setInstitutions)}>
      <SumiAppContainer screenProps={"auth": auth} />
    </Institution.Provider>
  </Auth.Provider>;
};
