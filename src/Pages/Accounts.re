open ReactNative;
open ReactNavigation;
open Institutions;

module Item = {
  let styles =
    Style.(
      StyleSheet.create({
        "container":
          style(
            ~alignItems=`center,
            ~alignSelf=`stretch,
            ~flex=1.,
            ~flexDirection=`row,
            ~justifyContent=`spaceBetween,
            ~padding=10.->dp,
            (),
          ),
        "button":
          style(
            ~marginTop=25.->dp,
            ~borderColor=GlobalStyles.colors.primary,
            ~borderWidth=2.,
            ~borderRadius=5.,
            (),
          ),
        "image":
          style(~resizeMode=`contain, ~width=20.->pct, ~height=40.->dp, ()),
        "text": style(~fontSize=24., ()),
      })
    );
  [@react.component]
  let make = (~name, ~image, ~institutionId, ~navigation: Navigation.t) => {
    let (auth, _) = React.useContext(Auth.context).Auth.auth;
    let user = auth->Auth.currentUserOrRaise;

    <TouchableHighlight
      style=styles##button
      onPress={
        _ =>
          Js.Promise.(
            Institutions.authorise(~userUuid=user.uuid, ~institutionId)
            |> then_(uri =>
                 navigation
                 ->Navigation.navigateWithParams(
                     "Webview",
                     {"uri": uri, "bankName": name},
                   )
                 ->resolve
               )
            |> ignore
          )
      }>
      <View style=styles##container>
        <Text style=styles##text> name->React.string </Text>
        <Image
          style=styles##image
          source=Image.(Source.fromUriSource(uriSource(~uri=image, ())))
        />
      </View>
    </TouchableHighlight>;
  };
};

module List = {
  let styles =
    Style.(
      StyleSheet.create({
        "listContainer": style(~padding=2.5->pct, ()),
        "heading":
          style(
            ~borderBottomWidth=1.,
            ~borderBottomColor=GlobalStyles.colors.primary,
            ~paddingBottom=10.->dp,
            (),
          ),
      })
    );
  [@react.component]
  let make = (~institutions: institutions, ~navigation: Navigation.t) =>
    <View
      style=Style.(
        [|GlobalStyles.styles##fullWidthContainer, styles##listContainer|]
        ->array
      )>
      <View style=styles##heading>
        <Text>
          "Select a bank to link to your Sumi account"->React.string
        </Text>
      </View>
      <FlatList
        data=institutions
        bounces=false
        keyExtractor={({id}, _) => id}
        renderItem={
          props =>
            <Item
              navigation
              institutionId={props##item.id}
              name={props##item.name}
              image={List.hd(props##item.media).source}
            />
        }
      />
    </View>;
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
      Institutions.getList()
      |> then_(list_ => setInstitutions(_ => list_->GotInstitutions)->resolve)
      |> catch(_err => setInstitutions(_ => AuthFailed)->resolve)
    );

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
        navigation->Navigation.navigate("SignIn");
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
