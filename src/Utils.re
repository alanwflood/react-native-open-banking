open ReactNative;
open ReactNavigation;

external elementToObj: TextInput.element => Js.t({..}) = "%identity";
let focusRef = ref =>
  ref
  ->React.Ref.current
  ->Js.Nullable.toOption
  ->Belt.Option.map(ref => ref->elementToObj##focus())
  ->ignore;

let tabBarIcon = element =>
  NavigationOptions.TabBarIcon.render(_props => element);

[@bs.module "search-params"]
external parseUrlParams: string => Js.t({..}) = "parse";
