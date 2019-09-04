// Generated by BUCKLESCRIPT VERSION 5.0.6, PLEASE EDIT WITH CARE

import * as Json from "@glennsl/bs-json/src/Json.bs.js";
import * as Block from "bs-platform/lib/es6/block.js";
import * as Fetch from "bs-fetch/src/Fetch.js";
import * as Caml_option from "bs-platform/lib/es6/caml_option.js";
import * as Json_decode from "@glennsl/bs-json/src/Json_decode.bs.js";
import * as Json_encode from "@glennsl/bs-json/src/Json_encode.bs.js";
import * as ReactNative from "react-native";
import * as Caml_exceptions from "bs-platform/lib/es6/caml_exceptions.js";

function stringToError(message) {
  switch (message) {
    case "Email address should be valid" : 
        return /* InvalidEmail */2;
    case "Network request failed" : 
        return /* NetworkFailure */0;
    case "Submitted credentials are incorrect" : 
        return /* InvalidCredentials */1;
    default:
      return /* UnknownError */[message];
  }
}

function user(json) {
  return /* Success */Block.__(0, [/* record */[
              /* token */Json_decode.field("token", (function (param) {
                      return Json_decode.withDefault("", Json_decode.string, param);
                    }), json),
              /* createdDate */Json_decode.field("createdDate", (function (param) {
                      return Json_decode.map((function (prim) {
                                    return new Date(prim);
                                  }), Json_decode.$$float, param);
                    }), json),
              /* name */Json_decode.field("name", Json_decode.string, json),
              /* id */Json_decode.field("id", Json_decode.$$int, json),
              /* uuid */Json_decode.field("uuid", Json_decode.string, json),
              /* email */Json_decode.field("email", Json_decode.string, json)
            ]]);
}

function error(json) {
  return /* Fail */Block.__(1, [Json_decode.field("message", (function (param) {
                    return Json_decode.map(stringToError, Json_decode.string, param);
                  }), json)]);
}

function login(json) {
  return Json_decode.either(user, error)(json);
}

function fetchError(_error) {
  return /* Fail */Block.__(1, [stringToError((_error.message))]);
}

var Decode = /* module */[
  /* stringToError */stringToError,
  /* user */user,
  /* error */error,
  /* login */login,
  /* fetchError */fetchError
];

function login$1(email, password) {
  var payload = JSON.stringify(Json_encode.object_(/* :: */[
            /* tuple */[
              "email",
              email
            ],
            /* :: */[
              /* tuple */[
                "password",
                password
              ],
              /* [] */0
            ]
          ]));
  return fetch("http://localhost:8080/api/authentications", Fetch.RequestInit[/* make */0](/* Post */2, {
                    "Content-Type": "application/json"
                  }, Caml_option.some(payload), undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined)(/* () */0));
}

var $$Request = /* module */[/* login */login$1];

var NoAuthToken = Caml_exceptions.create("User.Login.NoAuthToken");

function getAuthToken(response) {
  var match = response.headers.get("x-auth-token");
  if (match !== null) {
    return match;
  } else {
    throw [
          NoAuthToken,
          "No auth token found in server response"
        ];
  }
}

function user$1(user$2) {
  return Json_encode.object_(/* :: */[
              /* tuple */[
                "token",
                user$2[/* token */0]
              ],
              /* :: */[
                /* tuple */[
                  "createdDate",
                  user$2[/* createdDate */1].getTime()
                ],
                /* :: */[
                  /* tuple */[
                    "name",
                    user$2[/* name */2]
                  ],
                  /* :: */[
                    /* tuple */[
                      "id",
                      user$2[/* id */3]
                    ],
                    /* :: */[
                      /* tuple */[
                        "uuid",
                        user$2[/* uuid */4]
                      ],
                      /* :: */[
                        /* tuple */[
                          "email",
                          user$2[/* email */5]
                        ],
                        /* [] */0
                      ]
                    ]
                  ]
                ]
              ]
            ]);
}

var Encode = /* module */[/* user */user$1];

var Login = /* module */[
  /* Decode */Decode,
  /* Request */$$Request,
  /* NoAuthToken */NoAuthToken,
  /* getAuthToken */getAuthToken,
  /* Encode */Encode
];

function login$2(email, password) {
  var authToken = /* record */[/* contents */""];
  return login$1(email, password).then((function (response) {
                    authToken[0] = getAuthToken(response);
                    return response.json();
                  })).then((function (json) {
                  var user$2 = Json_decode.either(user, error)(json);
                  if (!user$2.tag) {
                    var userData = user$2[0];
                    ReactNative.AsyncStorage.setItem("user", Json.stringify(user$1(/* record */[
                                  /* token */authToken[0],
                                  /* createdDate */userData[/* createdDate */1],
                                  /* name */userData[/* name */2],
                                  /* id */userData[/* id */3],
                                  /* uuid */userData[/* uuid */4],
                                  /* email */userData[/* email */5]
                                ])));
                  }
                  return Promise.resolve(user$2);
                })).catch((function (error) {
                return Promise.resolve(fetchError(error));
              }));
}

export {
  Login ,
  login$2 as login,
  
}
/* react-native Not a pure module */
