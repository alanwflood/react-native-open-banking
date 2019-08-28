open ReactNavigation;

/* Routes to don't require authentication */
/* module PublicStack = {                                                    */
/*   let routes = {"SignIn": SignIn.make};                                   */
/*   let navigator =                                                         */
/*     StackNavigator.(                                                      */
/*       makeWithConfig(routes, config(~mode=`modal, ~headerMode=`none, ())) */
/*     );                                                                    */
/* };                                                                        */

/* Routes to enable authentication */
module AuthStack = {
  let routes = {"SignIn": SignIn.make};
  let navigator =
    StackNavigator.(
      makeWithConfig(routes, config(~mode=`modal, ~headerMode=`none, ()))
    );
};

/* Routes that require authentication */
module AppStack = {
  let routes = {"Accounts": Accounts.make, "Dashboard": Dashboard.make};
  let navigator = TabNavigator.Bottom.make(routes);
};

/* Merge the navigators together and by default load the
 * AuthLoading component to redirect based
 * on the user being logged in or not */
module SumiAppContainer =
  AppContainer.Make({
    type screenProps = {
      .
      "token": Auth.token,
      "auth": Auth.authType,
    };

    let navigator =
      SwitchNavigator.makeWithConfig(
        {
          "App": AppStack.navigator,
          "Auth": AuthStack.navigator,
          "AuthLoading": AuthLoading.make,
        },
        SwitchNavigator.config(~initialRouteName="AuthLoading", ()),
      );
  });
