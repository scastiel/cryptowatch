type asset = {
  id: int,
  symbol: string,
  name: string,
  fiat: Js.boolean,
  route: string
};

type pair = {
  id: int,
  symbol: string,
  base: asset,
  quote: asset,
  route: string
};

type market = {
  id: int,
  exchange: string,
  pair: string,
  active: Js.boolean,
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

let decodePairJson = (pairJson: Js.Json.t) : pair => {
  let pairDict = pairJson |> Js.Json.decodeObject |> Js.Option.getExn;
  {
    id: decodeInt(pairDict, "id"),
    symbol: decodeString(pairDict, "symbol"),
    base: Js.Dict.get(pairDict, "base") |> Js.Option.getExn |> decodeAssetJson,
    quote: Js.Dict.get(pairDict, "quote") |> Js.Option.getExn |> decodeAssetJson,
    route: decodeString(pairDict, "route")
  }
};

let decodeMarketJson = (marketJson: Js.Json.t) : market => {
  let marketDict = marketJson |> Js.Json.decodeObject |> Js.Option.getExn;
  {
    id: decodeInt(marketDict, "id"),
    exchange: decodeString(marketDict, "exchange"),
    pair: decodeString(marketDict, "pair"),
    active: decodeBoolean(marketDict, "active"),
    route: decodeString(marketDict, "route")
  }
};

let decodeAssetsJson = (json: Js.Json.t) : Js.Array.t(asset) => {
  let result =
    json
    |> Js.Json.decodeObject
    |> Js.Option.andThen([@bs] ((p) => Js.Dict.get(p, "result")))
    |> Js.Option.getExn;
  let assets = result |> Js.Json.decodeArray |> Js.Option.getExn;
  assets |> Js.Array.map(decodeAssetJson)
};

let decodePairsJson = (json: Js.Json.t) : Js.Array.t(pair) => {
  let result =
    json
    |> Js.Json.decodeObject
    |> Js.Option.andThen([@bs] ((p) => Js.Dict.get(p, "result")))
    |> Js.Option.getExn;
  let pairs = result |> Js.Json.decodeArray |> Js.Option.getExn;
  pairs |> Js.Array.map(decodePairJson)
};

let decodeMarketsJson = (json: Js.Json.t) : Js.Array.t(market) => {
  let result =
    json
    |> Js.Json.decodeObject
    |> Js.Option.andThen([@bs] ((p) => Js.Dict.get(p, "result")))
    |> Js.Option.getExn;
  let markets = result |> Js.Json.decodeArray |> Js.Option.getExn;
  markets |> Js.Array.map(decodeMarketJson)
};

let fetchAssets = () =>
  Js.Promise.(
    Fetch.fetch("https://api.cryptowat.ch/assets")
    |> then_(Fetch.Response.json)
    |> then_((json) => Js.log(json |> decodeAssetsJson) |> resolve)
  );

let fetchPairs = () =>
  Js.Promise.(
    Fetch.fetch("https://api.cryptowat.ch/pairs")
    |> then_(Fetch.Response.json)
    |> then_((json) => Js.log(json |> decodePairsJson) |> resolve)
  );

let fetchMarkets = () =>
  Js.Promise.(
    Fetch.fetch("https://api.cryptowat.ch/markets")
    |> then_(Fetch.Response.json)
    |> then_((json) => Js.log(json |> decodeMarketsJson) |> resolve)
  );

Js.Promise.(
  fetchMarkets()
  |> then_((assets) => Js.log(assets) |> resolve)
  |> catch((err) => Js.log(err) |> resolve)
);