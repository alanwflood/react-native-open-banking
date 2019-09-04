// Generated by BUCKLESCRIPT VERSION 5.0.6, PLEASE EDIT WITH CARE

import * as Auth from "../Context/Auth.bs.js";
import * as React from "react";
import * as LoadingScreen from "../Components/LoadingScreen.bs.js";

function AuthLoading(Props) {
  var navigation = Props.navigation;
  Props.countdown;
  var match = React.useContext(Auth.context)[/* auth */0];
  var setUser = match[1];
  React.useEffect((function () {
          Auth.checkAuthWithRoute(navigation, setUser);
          return (function (param) {
                    return /* () */0;
                  });
        }), ([]));
  return React.createElement(LoadingScreen.make, { });
}

var make = AuthLoading;

export {
  make ,
  
}
/* Auth Not a pure module */
