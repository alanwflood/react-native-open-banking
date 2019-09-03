open ReactNative;
open ReactNavigation;

let authSuccess = url =>
  switch (Js.String.match(Js.Re.fromString("localhost:8080"), url)) {
  | Some(_matches) => true
  | None => false
  };

[@react.component]
let make = (~navigation) => {
  /* Navigation Helpers */
  let getNavParam = param =>
    navigation->Navigation.getParamWithDefault(param, "");

  let uri = getNavParam("uri");
  let bankName: string = getNavParam("bankName");
  let goBackWithError = (~message: string) => {
    Alert.alert(~title="An Error Occured", ~message, ());
    navigation->Navigation.goBack;
  };
  let ammendError = () =>
    goBackWithError(
      ~message=
        bankName
        ++ "'s authentication system responded with an error, please try again later",
    );

  let ammendConsent = url => {
    let params = url->Utils.parseUrlParams;
    Js.log2("params recieved:", params);
    switch (params##error->Js.Nullable.toOption) {
    | Some(error) =>
      ammendError();
      Js.log2("Error in params:", error);
    | None =>
      Js.Promise.(
        Consents.ammend(
          ~userUuid=[%raw "params['user-uuid']"],
          ~institutionId=params##institution,
          ~consentToken=params##consent,
        )
        |> then_(data => data->Js.log->resolve)
        |> catch(_err => {
             ammendError();
             _err->Js.log->resolve;
           })
      )
      |> ignore
    };
  };

  <WebView
    source=WebView.(Source.uri(~uri, ()))
    onError={
      _event =>
        goBackWithError(
          ~message=
            "Failed to reach bank's authentication service, please try again later",
        )
    }
    renderLoading={_ => <LoadingScreen />}
    onShouldStartLoadWithRequest={
      request =>
        if (request##url->authSuccess) {
          request##url->ammendConsent;
          false;
        } else {
          true;
        }
    }
  />;
};
make
->NavigationOptions.setDynamicNavigationOptions(params => {
    let title =
      params##navigation->Navigation.getParamWithDefault("bankName", "");
    NavigationOptions.t(
      ~title,
      ~headerTitleStyle=Style.(style(~fontWeight=`bold, ())),
      ~headerTintColor=GlobalStyles.colors.textLight,
      ~headerStyle=
        Style.(style(~backgroundColor=GlobalStyles.colors.primary, ())),
      (),
    );
  });
