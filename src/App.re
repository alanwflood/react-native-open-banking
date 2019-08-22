open AppNavigator;

[@react.component]
let app = () => {
  let screenProps = {"someProp": 42};
  <SumiAppContainer screenProps />;
};
