open ReactNative;
open ReactNavigation;
open Institutions;

module CurrentAccounts = {
  [@react.component]
  let make = (~navigation) => {
    let (institutions, _) =
      React.useContext(Institution.context).Institution.institutions;

    switch (institutions) {
    | Institution.Got(institutions) =>
      let filteredInstitutions =
        List.filter(
          i => i.consentStatus != Consents.AwaitingAuthorization,
          institutions,
        );

      let (expiredAuth, withAuth) =
        filteredInstitutions
        |> List.partition(i => i.consentStatus == Consents.Expired);
      <AccountsList.List
        institutions={List.append(expiredAuth, withAuth)}
        heading={
          expiredAuth->List.length == 0 ?
            "Accounts you've linked with Sumi" :
            expiredAuth->List.length->string_of_int
            ++ " of your accounts require reauthorisation"
        }
        canAdd=true
        navigation
      />;
    | _ => React.null
    };
  };
  make
  ->NavigationOptions.setNavigationOptions(
      NavigationOptions.t(~title="Authorized", ()),
    );
};

module OtherAccounts = {
  [@react.component]
  let make = (~navigation) => {
    let (institutions, _) =
      React.useContext(Institution.context).Institution.institutions;

    switch (institutions) {
    | Institution.Got(institutions) =>
      <AccountsList.List
        institutions={
          List.filter(
            i => i.consentStatus == Consents.AwaitingAuthorization,
            institutions,
          )
        }
        canAdd=false
        heading="Select a bank to link it to your Sumi account"
        navigation
      />
    | _ => React.null
    };
  };
  make
  ->NavigationOptions.setNavigationOptions(
      NavigationOptions.t(~title="Other", ()),
    );
};

let navigator =
  TabNavigator.(
    MaterialTop.makeWithConfig(
      {
        "CurrentAccounts": CurrentAccounts.make,
        "OtherAccounts": OtherAccounts.make,
      },
      TabNavigator.config(
        ~tabBarOptions=
          tabBarOptions(
            ~activeTintColor=GlobalStyles.colors.primary,
            ~inactiveTintColor=GlobalStyles.colors.inactive,
            ~style=
              Style.style(~backgroundColor=GlobalStyles.colors.textLight, ()),
            ~indicatorStyle=
              Style.style(~backgroundColor=GlobalStyles.colors.primary, ()),
            (),
          ),
        (),
      ),
    )
  );

navigator
->NavigationOptions.(
    setNavigationOptions(
      t(
        ~headerTitle=
          NavigationOptions.HeaderTitle.element("Accounts"->React.string),
        ~headerTitleStyle=Style.(style(~fontWeight=`bold, ())),
        (),
      ),
    )
  );
