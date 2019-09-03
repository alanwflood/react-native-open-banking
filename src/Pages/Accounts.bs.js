// Generated by BUCKLESCRIPT VERSION 5.0.6, PLEASE EDIT WITH CARE

import * as Auth from "../Context/Auth.bs.js";
import * as List from "bs-platform/lib/es6/list.js";
import * as $$Array from "bs-platform/lib/es6/array.js";
import * as Curry from "bs-platform/lib/es6/curry.js";
import * as React from "react";
import * as GlobalStyles from "../GlobalStyles.bs.js";
import * as Institutions from "../API/Institutions.bs.js";
import * as ReactNative from "react-native";
import * as LoadingScreen from "../Components/LoadingScreen.bs.js";
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

function consentImage(status) {
  if (status !== 1) {
    if (status !== 5) {
      return null;
    } else {
      return React.createElement(VectorIcons.MaterialIcons, {
                  name: "refresh",
                  size: 32,
                  color: GlobalStyles.colors[/* warning */2]
                });
    }
  } else {
    return React.createElement(VectorIcons.MaterialIcons, {
                name: "check-circle",
                size: 24,
                color: GlobalStyles.colors[/* primary */1]
              });
  }
}

function Accounts$Item(Props) {
  var name = Props.name;
  var image = Props.image;
  var status = Props.status;
  var institutionId = Props.institutionId;
  var navigation = Props.navigation;
  var match = React.useContext(Auth.context)[/* auth */0];
  var user = Auth.currentUserOrRaise(match[0]);
  return React.createElement(ReactNative.TouchableHighlight, {
              style: styles.button,
              onPress: (function (param) {
                  Institutions.authorise(user[/* uuid */3], institutionId).then((function (uri) {
                          return Promise.resolve((navigation.navigate("Webview", {
                                            uri: uri,
                                            bankName: name
                                          }), /* () */0));
                        }));
                  return /* () */0;
                }),
              children: React.createElement(ReactNative.View, {
                    style: styles.container,
                    children: null
                  }, React.createElement(ReactNative.Text, {
                        style: styles.text,
                        children: name
                      }), React.createElement(ReactNative.Text, {
                        children: consentImage(status)
                      }), React.createElement(ReactNative.Image, {
                        source: {
                          uri: image
                        },
                        style: styles.image
                      }))
            });
}

var Item = /* module */[
  /* styles */styles,
  /* consentImage */consentImage,
  /* make */Accounts$Item
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

function Accounts$List(Props) {
  var institutions = Props.institutions;
  var navigation = Props.navigation;
  var institutionsList = function (institutions, heading) {
    var match = List.length(institutions) > 0;
    if (match) {
      return React.createElement(React.Fragment, undefined, React.createElement(ReactNative.View, {
                      style: styles$1.heading,
                      children: React.createElement(ReactNative.Text, {
                            children: heading
                          })
                    }), React.createElement(ReactNative.FlatList, {
                      data: $$Array.of_list(institutions),
                      keyExtractor: (function (param, param$1) {
                          return param[/* id */1];
                        }),
                      renderItem: (function (props) {
                          return React.createElement(Accounts$Item, {
                                      name: props.item[/* name */2],
                                      image: List.hd(props.item[/* media */4])[/* source */0],
                                      status: props.item[/* consentStatus */0],
                                      institutionId: props.item[/* id */1],
                                      navigation: navigation
                                    });
                        }),
                      bounces: false
                    }));
    } else {
      return null;
    }
  };
  var paritionedInstitutions = List.partition((function (i) {
          return i[/* consentStatus */0] === /* Expired */5;
        }), institutions);
  var reauth = institutionsList(paritionedInstitutions[0], "Some of your accounts required reauthorisation");
  var authed = institutionsList(paritionedInstitutions[1], "Select a bank to link it to your Sumi account");
  return React.createElement(ReactNative.View, {
              style: /* array */[
                GlobalStyles.styles.fullWidthContainer,
                styles$1.listContainer
              ],
              children: null
            }, reauth, authed);
}

var List$1 = /* module */[
  /* styles */styles$1,
  /* make */Accounts$List
];

function Accounts(Props) {
  var navigation = Props.navigation;
  var match = React.useState((function () {
          return /* Loading */0;
        }));
  var setInstitutions = match[1];
  var institutionsList = match[0];
  var fetchInstitutes = function (param) {
    return Institutions.get(/* () */0).then((function (list_) {
                    return Promise.resolve(Curry._1(setInstitutions, (function (param) {
                                      return /* GotInstitutions */[list_];
                                    })));
                  })).catch((function (_err) {
                  return Promise.resolve(Curry._1(setInstitutions, (function (param) {
                                    return /* AuthFailed */1;
                                  })));
                }));
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
      return React.createElement(LoadingScreen.make, { });
    }
  } else {
    return React.createElement(Accounts$List, {
                institutions: institutionsList[0],
                navigation: navigation
              });
  }
}

Accounts.navigationOptions = {
  title: "Home",
  headerTitle: "Accounts",
  headerTitleStyle: {
    fontWeight: "bold"
  }
};

var make = Accounts;

export {
  Item ,
  List$1 as List,
  make ,
  
}
/* styles Not a pure module */
