open ReactNative;
open ReactNavigation;
open Institutions;

module InstitutionItem = {
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
  let make = (~name, ~image) =>
    <TouchableHighlight style=styles##button>
      <View style=styles##container>
        <Text style=styles##text> name->React.string </Text>
        <Image
          style=styles##image
          source=Image.(Source.fromUriSource(uriSource(~uri=image, ())))
        />
      </View>
    </TouchableHighlight>;
};

module InstitutionsList = {
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
  let make = (~institutions: institutions) =>
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
            <InstitutionItem
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

[@react.component]
let make = (~navigation) => {
  let (auth, _) = React.useContext(Auth.context).Auth.auth;
  let (institutionsList, setInstitutions) = React.useState(() => Loading);
  let _user = auth->Auth.currentUserOrRaise;
  let fetchInstitutes = () =>
    Js.Promise.(
      Institutions.getList()
      |> then_(list_ => setInstitutions(_ => list_->GotInstitutions)->resolve)
      |> catch(_err => setInstitutions(_ => AuthFailed)->resolve)
      |> ignore
    );

  React.useEffect0(() => {
    fetchInstitutes();
    Some(() => ());
  });

  React.useEffect1(
    () => {
      if (institutionsList == AuthFailed) {
        navigation->Navigation.navigate("SignIn");
      };
      Some(() => ());
    },
    [|institutionsList|],
  );

  switch (institutionsList) {
  | Loading => <Text> "Loading"->React.string </Text>
  | AuthFailed => React.null
  | GotInstitutions(institutions) => <InstitutionsList institutions />
  };
};
make
->NavigationOptions.setNavigationOptions(
    NavigationOptions.t(
      ~tabBarIcon=
        NavigationOptions.TabBarIcon.render(_props =>
          <Icons.Material name="account-balance" size=22 />
        ),
      (),
    ),
  );
