type authType =
  | LoggedIn(User.Login.user)
  | LoggedOut;

let authToString = thing =>
  switch (thing) {
  | LoggedIn(_user) => "Logged in"
  | LoggedOut => "Logged out"
  };

type authContext = {auth: (authType, (authType => authType) => unit)};

let context = React.createContext({auth: (LoggedOut, _ => ())});

module Provider = {
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };
  let make = React.Context.provider(context);
};
