open ReactNative;

/*
 Here is StyleSheet that is using Style module to define styles for your components
 The main different with JavaScript components you may encounter in React Native
 is the fact that they **must** be defined before being referenced
 (so before actual component definitions)
 More at https://reasonml-community.github.io/reason-react-native/en/docs/apis/Style/
 */
let styles =
  Style.(
    StyleSheet.create({
      "container":
        style(
          ~flex=1.,
          ~justifyContent=`center,
          ~alignItems=`center,
          ~backgroundColor="#F5FCFF",
          (),
        ),
      "textInput":
        style(
          ~borderColor="#CCCCCC",
          ~borderBottomWidth=1.,
          ~height=50.->dp,
          ~fontSize=25.,
          ~paddingLeft=20.->dp,
          ~paddingRight=20.->dp,
          ~marginTop=15.->dp,
          ~width=75.->pct,
          (),
        ),
      "errorText": style(~color="#ff0033", ~marginTop=15.->dp, ()),
      "submitButton":
        style(
          ~marginTop=25.->dp,
          ~backgroundColor="#053CC8",
          ~width=75.->pct,
          (),
        ),
    })
  );

type state = {
  email: string,
  password: string,
  error: string,
};

let initialState = {email: "", password: "", error: " "};

type action =
  | SetEmail(string)
  | SetPassword(string)
  | SetError(string);

let submitForm = (email, password, dispatch) =>
  Js.Promise.(
    LoginAction.login(email, password)
    |> then_(result =>
         switch (result) {
         | LoginAction.Success(user) => user->Js.log->resolve
         | LoginAction.Fail(error) =>
           error.message->SetError->dispatch->resolve
         }
       )
    |> ignore
  );

external elementToObj: TextInput.element => Js.t({..}) = "%identity";

[@react.component]
let app = () => {
  let ref = React.useRef(Js.Nullable.null);

  let ({email, password, error}, dispatch) =
    React.useReducer(
      ({email, password, error}, action) =>
        switch (action) {
        | SetEmail(newEmail) => {email: newEmail, password, error}
        | SetPassword(newPassword) => {email, password: newPassword, error}
        | SetError(error) => {email, password, error}
        },
      initialState,
    );

  <View style=styles##container>
    <Text> "Sumi"->React.string </Text>
    <TextInput
      autoCapitalize=`none
      autoComplete=`email
      keyboardType=`emailAddress
      onChangeText={text => dispatch(SetEmail(text))}
      onSubmitEditing={
        _ =>
          ref
          ->React.Ref.current
          ->Js.Nullable.toOption
          ->Belt.Option.map(ref => ref->elementToObj##focus())
          ->ignore
      }
      placeholder="Email"
      returnKeyType=`next
      style=styles##textInput
      textContentType=`emailAddress
      value=email
    />
    <TextInput
      autoComplete=`password
      onChangeText={text => dispatch(SetPassword(text))}
      /* onSubmitEditing={_ => dispatch(SubmitLogin)} */
      placeholder="Password"
      ref
      returnKeyType=`go
      secureTextEntry=true
      style=styles##textInput
      textContentType=`password
      value=password
    />
    <Text style=styles##errorText> error->React.string </Text>
    <View style=styles##submitButton>
      <Button
        title="Sign In"
        onPress={_ => submitForm(email, password, dispatch)}
        color="#ffffff"
        accessibilityLabel="Learn more about this purple button"
      />
    </View>
  </View>;
};
