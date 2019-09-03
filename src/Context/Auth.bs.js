// Generated by BUCKLESCRIPT VERSION 5.0.6, PLEASE EDIT WITH CARE

import * as Json from "@glennsl/bs-json/src/Json.bs.js";
import * as User from "../API/User.bs.js";
import * as Curry from "bs-platform/lib/es6/curry.js";
import * as React from "react";
import * as ReactNative from "react-native";
import * as Caml_exceptions from "bs-platform/lib/es6/caml_exceptions.js";

var RetrieveUserError = Caml_exceptions.create("Auth.RetrieveUserError");

function mapUserLoginToAuth(loginStatus) {
  if (loginStatus.tag) {
    return /* LoggedOut */0;
  } else {
    return /* LoggedIn */[loginStatus[0]];
  }
}

function isLoggedIn(auth) {
  if (auth) {
    return true;
  } else {
    return false;
  }
}

function currentUserOrRaise(auth) {
  if (auth) {
    return auth[0];
  } else {
    throw [
          RetrieveUserError,
          "Current User missing"
        ];
  }
}

var RetrieveUserError$1 = Caml_exceptions.create("Auth.RetrieveUserError");

function getCurrentUser(param) {
  return ReactNative.AsyncStorage.getItem("user").then((function (json) {
                if (json !== null) {
                  return Promise.resolve(mapUserLoginToAuth(User.Login[/* decodeUser */1](Json.parseOrRaise(json))));
                } else {
                  return Promise.resolve(/* LoggedOut */0);
                }
              }));
}

var RetrieveTokenError = Caml_exceptions.create("Auth.RetrieveTokenError");

function getAuthToken(param) {
  return ReactNative.AsyncStorage.getItem("authToken").then((function (nullableToken) {
                if (nullableToken !== null) {
                  return Promise.resolve(nullableToken);
                } else {
                  return Promise.reject([
                              RetrieveTokenError,
                              "Token not found"
                            ]);
                }
              }));
}

function logOut(navigation) {
  ReactNative.AsyncStorage.clear().then((function (_result) {
          return Promise.resolve((navigation.navigate("SignIn"), /* () */0));
        }));
  return /* () */0;
}

function checkAuthWithRoute(navigation, setAuth, setToken) {
  return Promise.all(/* array */[
                  getCurrentUser(/* () */0).then((function (user) {
                          return Promise.resolve(Curry._1(setAuth, (function (param) {
                                            return user;
                                          })));
                        })),
                  getAuthToken(/* () */0).then((function (token) {
                          return Promise.resolve(Curry._1(setToken, (function (param) {
                                            return token;
                                          })));
                        }))
                ]).then((function (param) {
                  return Promise.resolve((navigation.navigate("App"), /* () */0));
                })).catch((function (_err) {
                return Promise.resolve(logOut(navigation));
              }));
}

var context = React.createContext(/* record */[
      /* auth : tuple */[
        /* LoggedOut */0,
        (function (param) {
            return /* () */0;
          })
      ],
      /* token : tuple */[
        "",
        (function (param) {
            return /* () */0;
          })
      ]
    ]);

function makeProps(value, children, param) {
  return {
          value: value,
          children: children
        };
}

var make = context.Provider;

var Provider = /* module */[
  /* makeProps */makeProps,
  /* make */make
];

export {
  mapUserLoginToAuth ,
  isLoggedIn ,
  currentUserOrRaise ,
  RetrieveUserError$1 as RetrieveUserError,
  getCurrentUser ,
  RetrieveTokenError ,
  getAuthToken ,
  logOut ,
  checkAuthWithRoute ,
  context ,
  Provider ,
  
}
/* context Not a pure module */
