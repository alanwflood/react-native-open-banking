// Generated by BUCKLESCRIPT VERSION 5.0.6, PLEASE EDIT WITH CARE

import * as Auth from "./Context/Auth.bs.js";
import * as List from "bs-platform/lib/es6/list.js";
import * as Curry from "bs-platform/lib/es6/curry.js";
import * as React from "react";
import * as GlobalStyles from "./GlobalStyles.bs.js";
import * as Institutions from "./API/Institutions.bs.js";
import * as ReactNative from "react-native";
import * as Style$ReactNative from "reason-react-native/src/apis/Style.bs.js";
import * as VectorIcons from "@expo/vector-icons";

var styles = ReactNative.StyleSheet.create({
      container: {
        alignItems: "center",
        alignSelf: "stretch",
        flex: 1,
        flexDirection: "row",
        justifyContent: "space-between",
        padding: 10
      },
      button: {
        borderColor: GlobalStyles.colors[/* primary */1],
        borderRadius: 5,
        borderWidth: 2,
        marginTop: 25
      },
      image: {
        resizeMode: "contain",
        height: 40,
        width: Style$ReactNative.pct(20)
      },
      text: {
        fontSize: 24
      }
    });

function Accounts$InstitutionItem(Props) {
  var name = Props.name;
  var image = Props.image;
  return React.createElement(ReactNative.TouchableHighlight, {
              style: styles.button,
              children: React.createElement(ReactNative.View, {
                    style: styles.container,
                    children: null
                  }, React.createElement(ReactNative.Text, {
                        style: styles.text,
                        children: name
                      }), React.createElement(ReactNative.Image, {
                        source: {
                          uri: image
                        },
                        style: styles.image
                      }))
            });
}

var InstitutionItem = /* module */[
  /* styles */styles,
  /* make */Accounts$InstitutionItem
];

var styles$1 = ReactNative.StyleSheet.create({
      listContainer: {
        padding: Style$ReactNative.pct(2.5)
      },
      heading: {
        borderBottomColor: GlobalStyles.colors[/* primary */1],
        borderBottomWidth: 1,
        paddingBottom: 10
      }
    });

function Accounts$InstitutionsList(Props) {
  var institutions = Props.institutions;
  return React.createElement(ReactNative.View, {
              style: /* array */[
                GlobalStyles.styles.fullWidthContainer,
                styles$1.listContainer
              ],
              children: null
            }, React.createElement(ReactNative.View, {
                  style: styles$1.heading,
                  children: React.createElement(ReactNative.Text, {
                        children: "Select a bank to link to your Sumi account"
                      })
                }), React.createElement(ReactNative.FlatList, {
                  data: institutions,
                  keyExtractor: (function (param, param$1) {
                      return param[/* id */0];
                    }),
                  renderItem: (function (props) {
                      return React.createElement(Accounts$InstitutionItem, {
                                  name: props.item[/* name */1],
                                  image: List.hd(props.item[/* media */3])[/* source */0]
                                });
                    }),
                  bounces: false
                }));
}

var InstitutionsList = /* module */[
  /* styles */styles$1,
  /* make */Accounts$InstitutionsList
];

function Accounts(Props) {
  var navigation = Props.navigation;
  var match = React.useContext(Auth.context)[/* auth */0];
  var match$1 = React.useState((function () {
          return /* Loading */0;
        }));
  var setInstitutions = match$1[1];
  var institutionsList = match$1[0];
  Auth.currentUserOrRaise(match[0]);
  var fetchInstitutes = function (param) {
    Institutions.getList(/* () */0).then((function (list_) {
              return Promise.resolve(Curry._1(setInstitutions, (function (param) {
                                return /* GotInstitutions */[list_];
                              })));
            })).catch((function (_err) {
            return Promise.resolve(Curry._1(setInstitutions, (function (param) {
                              return /* AuthFailed */1;
                            })));
          }));
    return /* () */0;
  };
  React.useEffect((function () {
          fetchInstitutes(/* () */0);
          return (function (param) {
                    return /* () */0;
                  });
        }), ([]));
  React.useEffect((function () {
          if (institutionsList === /* AuthFailed */1) {
            navigation.navigate("SignIn");
          }
          return (function (param) {
                    return /* () */0;
                  });
        }), /* array */[institutionsList]);
  if (typeof institutionsList === "number") {
    if (institutionsList !== 0) {
      return null;
    } else {
      return React.createElement(ReactNative.Text, {
                  children: "Loading"
                });
    }
  } else {
    return React.createElement(Accounts$InstitutionsList, {
                institutions: institutionsList[0]
              });
  }
}

Accounts.navigationOptions = {
  tabBarIcon: (function (_props) {
      return React.createElement(VectorIcons.MaterialIcons, {
                  name: "account-balance",
                  size: 22
                });
    })
};

var make = Accounts;

export {
  InstitutionItem ,
  InstitutionsList ,
  make ,
  
}
/* styles Not a pure module */
