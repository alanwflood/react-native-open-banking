type institutions =
  | NotAsked
  | Loading
  | Failed
  | Got(Institutions.institutions);

type institutionContext = {
  institutions: (institutions, (institutions => institutions) => unit),
};

let context = React.createContext({institutions: (NotAsked, _ => ())});

module Provider = {
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };
  let make = React.Context.provider(context);
};
