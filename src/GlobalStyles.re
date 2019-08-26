open ReactNative;
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
      "headingText": style(~fontSize=20., ~marginTop=25.->dp, ()),
    })
  );
