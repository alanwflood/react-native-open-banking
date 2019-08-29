open ReactNative;
open ReactNavigation;
open User.Login;

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
          ~fontSize=20.,
          ~paddingLeft=20.->dp,
          ~paddingRight=20.->dp,
          ~marginTop=15.->dp,
          ~width=85.->pct,
          (),
        ),
      "errorText": style(~color="#ff0033", ~marginTop=15.->dp, ()),
      "submitButton":
        style(
          ~marginTop=25.->dp,
          ~backgroundColor="#053CC8",
          ~width=85.->pct,
          (),
        ),
    })
  );

type loginFields = {
  email: string,
  password: string,
};

type state = {
  loginFields,
  loading: bool,
  error: option(User.Login.error),
};

let initialState = {
  loginFields: {
    email: "",
    password: "",
  },
  loading: false,
  error: None,
};

type action =
  | SetEmail(string)
  | SetPassword(string)
  | SetError(option(error));

let signInError = optionalError =>
  React.string(
    switch (optionalError) {
    | Some(error) =>
      switch (error) {
      | InvalidCredentials => "Wrong username / password combination"
      | InvalidEmail => "Email address is not valid"
      | RequiredEmail => "Email cannot be empty"
      | RequiredPassword => "Password cannot be empty"
      | NetworkFailure => "Network is unreachable (Server is probably down?)"
      | UnknownError(error) => error
      }
    | None => " "
    },
  );

[@react.component]
let make = (~navigation) => {
  let ref = React.useRef(Js.Nullable.null);
  let ({loginFields: {email, password}, error}, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | SetEmail(email) => {
            ...state,
            loginFields: {
              ...state.loginFields,
              email,
            },
          }
        | SetPassword(password) => {
            ...state,
            loginFields: {
              ...state.loginFields,
              password,
            },
          }
        | SetError(error) => {...state, error}
        },
      initialState,
    );

  let submitForm = () =>
    Js.Promise.(
      (
        switch (String.trim(email), String.trim(password)) {
        | ("", "") => RequiredEmail->Some->SetError->dispatch->resolve
        | (_, "") => RequiredPassword->Some->SetError->dispatch->resolve
        | (_, _) =>
          User.login(~email, ~password)
          |> then_(result =>
               resolve(
                 switch (result) {
                 | Success(_user) =>
                   navigation->Navigation.navigate("AuthLoading")
                 | Fail(error) => error->Some->SetError->dispatch
                 },
               )
             )
        }
      )
      |> ignore
    );

  <View style=styles##container>
    <Text> "Sumi"->React.string </Text>
    <TextInput
      autoCapitalize=`none
      autoComplete=`email
      keyboardType=`emailAddress
      onChangeText={text => text->SetEmail->dispatch}
      onSubmitEditing={_ => ref->Utils.focusRef}
      placeholder="Email"
      returnKeyType=`next
      style=styles##textInput
      textContentType=`emailAddress
      value=email
    />
    <TextInput
      autoComplete=`password
      onChangeText={text => text->SetPassword->dispatch}
      onSubmitEditing={_ => submitForm()}
      placeholder="Password"
      ref
      returnKeyType=`go
      secureTextEntry=true
      style=styles##textInput
      textContentType=`password
      value=password
    />
    <Text style=styles##errorText> error->signInError </Text>
    <View style=styles##submitButton>
      <Button title="Sign In" onPress={_ => submitForm()} color="#ffffff" />
    </View>
  </View>;
};
