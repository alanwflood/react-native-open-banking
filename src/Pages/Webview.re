open ReactNative;
open ReactNavigation;

let authSuccess = url =>
  switch (Js.String.match(Js.Re.fromString("localhost:8080"), url)) {
  | Some(_matches) => true
  | None => false
  };

[@react.component]
let make = (~navigation) => {
  let goBack = () => navigation->Navigation.goBack;

  let uri =
    switch (navigation->Navigation.getParam("uri")->Js.Nullable.toOption) {
    | Some(uri) => uri
    | None =>
      goBack();
      "";
    };

  <WebView
    source=WebView.(Source.uri(~uri, ()))
    onError={
      event => {
        Js.log(event);
        navigation->Navigation.navigate("Accounts");
      }
    }
    renderLoading={_ => <LoadingScreen />}
    onShouldStartLoadWithRequest={
      e => {
        if (e##url->authSuccess) {
          Js.log(e##url->Utils.parseUrlParams);
        };
        true;
      }
    }
  />;
};
