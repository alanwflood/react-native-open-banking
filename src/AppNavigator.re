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

module AccountsStack = {
  let routes = {"Accounts": Accounts.make, "Webview": Webview.make};
  let navigator = StackNavigator.make(routes);
  navigator
  ->NavigationOptions.setNavigationOptions(
      NavigationOptions.t(
        ~tabBarIcon=
          NavigationOptions.TabBarIcon.render(props =>
            <Icons.Material
              name="account-balance"
              size=22
              color=props##tintColor
            />
          ),
        (),
      ),
    );
};

/* Routes that require authentication */
module AppStack = {
  let routes = {
    "Accounts": AccountsStack.navigator,
    "Dashboard": Dashboard.make,
  };
  let navigator =
    TabNavigator.(
      Bottom.(
        makeWithConfig(
          routes,
          config(
            ~tabBarOptions=
              tabBarOptions(~activeTintColor=GlobalStyles.colors.primary, ()),
            (),
          ),
        )
      )
    );
};

/* Merge the navigators together and by default load the
 * AuthLoading component to redirect based
 * on the user being logged in or not */
module SumiAppContainer =
  AppContainer.Make({
    type screenProps = {. "auth": Auth.authType};

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
