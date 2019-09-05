// Generated by BUCKLESCRIPT VERSION 5.0.6, PLEASE EDIT WITH CARE

import * as React from "react";
import * as Consents from "../API/Consents.bs.js";
import * as GlobalStyles from "../GlobalStyles.bs.js";
import * as ReactNative from "react-native";
import * as LoadingScreen from "../Components/LoadingScreen.bs.js";
import * as SearchParams from "search-params";

function authSuccess(url) {
  var match = url.match(new RegExp("localhost:8080"));
  return match !== null;
}

function Webview(Props) {
  var navigation = Props.navigation;
  var uri = navigation.getParam("uri", "");
  var bankName = navigation.getParam("bankName", "");
  var goBackWithError = function (message) {
    ReactNative.Alert.alert("An Error Occured", message, undefined, undefined, undefined);
    navigation.goBack();
    return /* () */0;
  };
  var ammendConsent = function (url) {
    var params = SearchParams.parse(url);
    console.log("params recieved:", params);
    var match = params.error;
    if (match == null) {
      Consents.ammend((params['user-uuid']), params.institution, params.consent, true).then((function (data) {
                return Promise.resolve((navigation.navigate("AccountsLoading"), /* () */0));
              })).catch((function (error) {
              goBackWithError(bankName + "'s authentication system responded with an error, please try again later");
              return Promise.resolve((console.log("Error in ammending consent:", error), /* () */0));
            }));
      return /* () */0;
    } else {
      goBackWithError(bankName + "'s authentication system responded with an error, please try again later");
      console.log("Error in params:", match);
      return /* () */0;
    }
  };
  return React.createElement(ReactNative.WebView, {
              onError: (function (_event) {
                  return goBackWithError("Failed to reach provider's authentication service, please try again later");
                }),
              onShouldStartLoadWithRequest: (function (request) {
                  if (authSuccess(request.url)) {
                    ammendConsent(request.url);
                    return false;
                  } else {
                    return true;
                  }
                }),
              renderLoading: (function (param) {
                  return React.createElement(LoadingScreen.make, { });
                }),
              source: {
                uri: uri
              }
            });
}

Webview.navigationOptions = (function (params) {
    var title = params.navigation.getParam("bankName", "");
    return {
            title: title,
            headerTitleStyle: {
              fontWeight: "bold"
            },
            headerTintColor: GlobalStyles.colors[/* textLight */5],
            headerStyle: {
              backgroundColor: GlobalStyles.colors[/* primary */1]
            }
          };
  });

var make = Webview;

export {
  authSuccess ,
  make ,
  
}
/*  Not a pure module */
