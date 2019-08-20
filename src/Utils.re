open ReactNative;

external elementToObj: TextInput.element => Js.t({..}) = "%identity";
let focusRef = ref =>
  ref
  ->React.Ref.current
  ->Js.Nullable.toOption
  ->Belt.Option.map(ref => ref->elementToObj##focus())
  ->ignore;
