open ReactNavigation;
open Institution;

[@react.component]
let make = (~navigation: Navigation.t, ~countdown=3) => {
  let (_ins, setInstitutions) =
    React.useContext(Institution.context).Institution.institutions;

  let updateInstitutions = () =>
    Js.Promise.(
      Institutions.get()
      |> then_(ins => {
           setInstitutions(_ => ins->Got);
           navigation->Navigation.navigate("Accounts")->resolve;
         })
      |> catch(error => {
           Js.log(error);
           setInstitutions(_ => Failed);
           Auth.logOut(~navigation)->resolve;
         })
    )
    |> ignore;

  React.useEffect0(() => {
    setInstitutions(_ => Loading);
    updateInstitutions();
    Some(() => ());
  });

  <LoadingScreen />;
};
