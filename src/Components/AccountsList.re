open ReactNative;
open ReactNavigation;

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

  let consentImage = (status: Consents.consentStatus) =>
    Consents.(
      switch (status) {
      | Expired =>
        <Icons.Material
          name="refresh"
          size=32
          color={GlobalStyles.colors.warning}
        />
      | Authorized =>
        <Icons.Material
          name="check-circle"
          size=24
          color={GlobalStyles.colors.primary}
        />
      | _ => React.null
      }
    );

  [@react.component]
  let make =
      (~name, ~image, ~status, ~institutionId, ~navigation: Navigation.t) => {
    let (auth, _) = React.useContext(Auth.context).Auth.auth;
    let user = auth->Auth.currentUserOrRaise;

    <TouchableHighlight
      underlayColor={GlobalStyles.colors.primary}
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
        <Text> status->consentImage </Text>
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
        "list_": style(~paddingBottom=45.->dp, ()),
        "heading":
          style(
            ~borderBottomColor=GlobalStyles.colors.primary,
            ~borderBottomWidth=1.,
            ~fontWeight=`bold,
            ~paddingBottom=5.->dp,
            (),
          ),
      })
    );

  [@react.component]
  let make =
      (
        ~institutions: Institutions.institutions,
        ~navigation: Navigation.t,
        ~heading,
      ) =>
    /* This shouldn't show if no institutions are passed */
    institutions->List.length > 0 ?
      <View style=styles##list_>
        <View style=styles##heading>
          <Text style=styles##heading> heading->React.string </Text>
        </View>
        <View>
          <FlatList
            data=institutions->Array.of_list
            bounces=false
            keyExtractor={({id}, _) => id}
            renderItem={
              props =>
                <Item
                  navigation
                  institutionId={props##item.id}
                  name={props##item.name}
                  status={props##item.consentStatus}
                  image={List.hd(props##item.media).source}
                />
            }
          />
        </View>
      </View> :
      React.null;
};
