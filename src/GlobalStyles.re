open ReactNative;

type colors = {
  background: string,
  primary: string,
  textDefault: string,
  textLight: string,
  textDark: string,
};

let colors = {
  background: "#F5FCFF",
  primary: "#2196f3",
  textDefault: "black",
  textLight: "#fafafa",
  textDark: "#212121",
};

let styles =
  Style.(
    StyleSheet.create({
      "container":
        style(
          ~flex=1.,
          ~justifyContent=`center,
          ~alignItems=`center,
          ~backgroundColor=colors.background,
          (),
        ),
      "fullWidthContainer":
        style(~backgroundColor=colors.background, ~flex=1., ()),
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
      "headingText": style(~fontSize=20., ~marginTop=25.->dp, ()),
    })
  );
