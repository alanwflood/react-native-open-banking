module Ionicons = {
  [@bs.module "@expo/vector-icons"] [@react.component]
  external make:
    (
      ~children: React.element=?,
      ~name: string=?,
      ~size: int=?,
      ~color: string=?,
      ~style: ReactNative.Style.t=?
    ) =>
    React.element =
    "Ionicons";
};

module AntDesign = {
  [@bs.module "@expo/vector-icons"] [@react.component]
  external make:
    (
      ~children: React.element=?,
      ~name: string=?,
      ~size: int=?,
      ~color: string=?,
      ~style: ReactNative.Style.t=?
    ) =>
    React.element =
    "AntDesign";
};

module Material = {
  [@bs.module "@expo/vector-icons"] [@react.component]
  external make:
    (
      ~children: React.element=?,
      ~name: string=?,
      ~size: int=?,
      ~color: string=?,
      ~style: ReactNative.Style.t=?
    ) =>
    React.element =
    "MaterialIcons";
};
