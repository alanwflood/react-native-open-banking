// Generated by BUCKLESCRIPT VERSION 5.0.6, PLEASE EDIT WITH CARE

import * as Json from "@glennsl/bs-json/src/Json.bs.js";
import * as User from "../API/User.bs.js";
import * as Curry from "bs-platform/lib/es6/curry.js";
import * as React from "react";
import * as ReactNative from "react-native";
import * as Caml_exceptions from "bs-platform/lib/es6/caml_exceptions.js";

var RetrieveUserError = Caml_exceptions.create("Auth.RetrieveUserError");

function currentUser(auth) {
  if (auth) {
    return auth[0];
  } else {
    throw [
          RetrieveUserError,
          "user not found"
        ];
  }
}

function authToString(thing) {
  if (thing) {
    return "Logged in";
  } else {
    return "Logged out";
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

function getCurrentUser(setAuth) {
  return ReactNative.AsyncStorage.getItem("user").then((function (json) {
                    if (json !== null) {
                      return Promise.resolve(User.Login[/* decodeUser */1](Json.parseOrRaise(json)));
                    } else {
                      throw [
                            RetrieveUserError$1,
                            "Error retrieving user from async storage"
                          ];
                    }
                  })).then((function (user) {
                  if (user.tag) {
                    return Promise.resolve(Curry._1(setAuth, (function (param) {
                                      return /* LoggedOut */0;
                                    })));
                  } else {
                    var user$1 = user[0];
                    return Promise.resolve(Curry._1(setAuth, (function (param) {
                                      return /* LoggedIn */[user$1];
                                    })));
                  }
                })).catch((function (_error) {
                return Promise.resolve(Curry._1(setAuth, (function (param) {
                                  return /* LoggedOut */0;
                                })));
              }));
}

function getAuthToken(navigation) {
  return ReactNative.AsyncStorage.getItem("authToken").then((function (nullableToken) {
                console.log("CHECKING_AUTH_TOKEN", nullableToken);
                return Promise.resolve(nullableToken !== null ? nullableToken : (navigation.navigate("SignIn"), ""));
              }));
}

function checkAuthWithRoute(navigation, setAuth, setToken) {
  return Promise.all(/* array */[
                getCurrentUser(setAuth),
                getAuthToken(navigation).then((function (token) {
                        return Promise.resolve(Curry._1(setToken, (function (param) {
                                          return token;
                                        })));
                      }))
              ]).then((function (param) {
                return Promise.resolve((navigation.navigate("App"), /* () */0));
              }));
}

function logOut(navigation) {
  ReactNative.AsyncStorage.clear().then((function (_result) {
          return Promise.resolve((navigation.navigate("SignIn"), /* () */0));
        }));
  return /* () */0;
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
  currentUser ,
  authToString ,
  isLoggedIn ,
  currentUserOrRaise ,
  RetrieveUserError$1 as RetrieveUserError,
  getCurrentUser ,
  getAuthToken ,
  checkAuthWithRoute ,
  logOut ,
  context ,
  Provider ,
  
}
/* context Not a pure module */
