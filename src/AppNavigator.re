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
  let routes = {"Accounts": Accounts.navigator, "Webview": Webview.make};
  let accountsNavigator =
    StackNavigator.make(routes)
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

  let navigator =
    SwitchNavigator.makeWithConfig(
      {
        "AccountsLoading": AccountsLoading.make,
        "Accounts": StackNavigator.make(routes),
      },
      SwitchNavigator.config(~initialRouteName="AccountsLoading", ()),
    );

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
              tabBarOptions(
                ~activeTintColor=GlobalStyles.colors.primary,
                ~inactiveTintColor=GlobalStyles.colors.inactive,
                (),
              ),
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
    type screenProps = {. "auth": Auth.authorization};

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
