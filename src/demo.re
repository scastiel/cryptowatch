type asset = {
  id: int,
  symbol: string,
  name: string,
  fiat: Js.boolean,
  route: string
};

let decodeInt = (dict, key) =>
  Js.Dict.get(dict, key)
  |> Js.Option.andThen([@bs] ((json) => Js.Json.decodeNumber(json)))
  |> Js.Option.getExn
  |> int_of_float;

let decodeString = (dict, key) =>
  Js.Dict.get(dict, key)
  |> Js.Option.andThen([@bs] ((json) => Js.Json.decodeString(json)))
  |> Js.Option.getExn;

let decodeBoolean = (dict, key) =>
  Js.Dict.get(dict, key)
  |> Js.Option.andThen([@bs] ((json) => Js.Json.decodeBoolean(json)))
  |> Js.Option.getExn;

let decodeAssetJson = (assetJson: Js.Json.t) : asset => {
  let assetDict = assetJson |> Js.Json.decodeObject |> Js.Option.getExn;
  {
    id: decodeInt(assetDict, "id"),
    symbol: decodeString(assetDict, "symbol"),
    name: decodeString(assetDict, "name"),
    fiat: decodeBoolean(assetDict, "fiat"),
    route: decodeString(assetDict, "route")
  }
};

let decodeAssetJson = (json: Js.Json.t) : Js.Array.t(asset) => {
  let result =
    json
    |> Js.Json.decodeObject
    |> Js.Option.andThen([@bs] ((p) => Js.Dict.get(p, "result")))
    |> Js.Option.getExn;
  let assets = result |> Js.Json.decodeArray |> Js.Option.getExn;
  assets |> Js.Array.map(decodeAssetJson)
};

let fetchAssets = () =>
  Js.Promise.(
    Fetch.fetch("https://api.cryptowat.ch/assets")
    |> then_(Fetch.Response.json)
    |> then_((json) => Js.log(json |> decodeAssetJson) |> resolve)
  );

Js.Promise.(fetchAssets() |> then_((assets) => Js.log(assets) |> resolve));