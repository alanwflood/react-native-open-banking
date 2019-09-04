open ReactNative;
open ReactNavigation;
open Institutions;

module List = {
  let styles =
    Style.(
      StyleSheet.create({
        "listContainer":
          style(~justifyContent=`flexStart, ~padding=2.5->pct, ()),
      })
    );
  [@react.component]
  let make = (~institutions, ~navigation: Navigation.t) => {
    let (awaitingAuth, otherInstitutions) =
      List.partition(
        i => i.consentStatus == Consents.AwaitingAuthorization,
        institutions,
      );

    let (expiredAuth, withAuth) =
      otherInstitutions
      |> List.partition(i => i.consentStatus == Consents.Expired);

    <View
      style=Style.(
        [|GlobalStyles.styles##fullWidthContainer, styles##listContainer|]
        ->array
      )>
      <AccountsList.List
        institutions={List.append(expiredAuth, withAuth)}
        heading={
          expiredAuth->List.length == 0 ?
            "Accounts you've linked with Sumi" :
            expiredAuth->List.length->string_of_int
            ++ " of your accounts require reauthorisation"
        }
        navigation
      />
      <AccountsList.List
        institutions=awaitingAuth
        heading=" Select a bank to link it to your Sumi account "
        navigation
      />
    </View>;
  };
};

type state =
  | Loading
  | AuthFailed
  | GotInstitutions(institutions);

type instituteError = option(string);

[@react.component]
let make = (~navigation) => {
  let (institutionsList, setInstitutions) = React.useState(() => Loading);
  let fetchInstitutes = () =>
    Js.Promise.(
      Institutions.get()
      |> then_(list_ => setInstitutions(_ => list_->GotInstitutions)->resolve)
      |> catch(error => {
           Js.log(error);
           setInstitutions(_ => AuthFailed)->resolve;
         })
    )
    |> ignore;

  React.useEffect0(() => {
    /* Fetch institutions on component mount */
    fetchInstitutes();
    Some(() => ());
  });

  React.useEffect1(
    () => {
      /* If we failed to get the institutions auth has timed out (Probably) */
      /* TODO: Add additional state type if server is down */
      if (institutionsList == AuthFailed) {
        Auth.logOut(~navigation);
      };
      Some(() => ());
    },
    [|institutionsList|],
  );

  switch (institutionsList) {
  | Loading => <LoadingScreen />
  | AuthFailed => React.null
  | GotInstitutions(institutions) => <List institutions navigation />
  };
};
make
->NavigationOptions.(
    setNavigationOptions(
      t(
        ~headerTitle=
          NavigationOptions.HeaderTitle.element("Accounts"->React.string),
        ~headerTitleStyle=Style.(style(~fontWeight=`bold, ())),
        ~title="Home",
        (),
      ),
    )
  );
