open ReactNative;
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
          ~width=75.->pct,
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
  | SetLoading(bool)
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
let app = () => {
  let ref = React.useRef(Js.Nullable.null);

  let ({loginFields: {email, password}, error}, dispatch) =
    React.useReducer(
      ({loginFields: f, error, loading}, action) =>
        switch (action) {
        | SetEmail(email) => {
            loginFields: {
              ...f,
              email,
            },
            loading,
            error,
          }
        | SetPassword(password) => {
            loginFields: {
              ...f,
              password,
            },
            loading,
            error,
          }
        | SetLoading(loading) => {loginFields: f, error, loading}
        | SetError(error) => {loginFields: f, error, loading}
        },
      initialState,
    );

  let submitForm = () =>
    Js.Promise.(
      (
        switch (String.trim(email), String.trim(password)) {
        | ("", "") => RequiredEmail->Some->resolve
        | (_, "") => RequiredPassword->Some->resolve
        | (_, _) =>
          User.Login.login(~email, ~password)
          |> then_(result =>
               resolve(
                 switch (result) {
                 | Success(_user) => None
                 | Fail(error) => error->Some
                 },
               )
             )
        }
      )
      |> then_(msg => msg->SetError->dispatch->resolve)
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
      <Button
        title="Sign In"
        onPress={_ => submitForm()}
        color="#ffffff"
        accessibilityLabel="Learn more about this purple button"
      />
    </View>
  </View>;
};
