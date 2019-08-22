open ReactNavigation;

module SumiAppContainer =
  AppContainer.Make({
    type screenProps = {. "someProp": int};

    let navigator =
      SwitchNavigator.make({
        "SignIn": StackNavigator.make({"SignIn": SignIn.make}),
        "Dashboard": StackNavigator.make({"Dashboard": Dashboard.make}),
      });
  });
