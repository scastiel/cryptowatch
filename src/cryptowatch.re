open Utils;

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

type exchange = {
  symbol: string,
  name: string,
  route: string,
  active: Js.boolean
};

let _decodeAssetJson = (assetJson: Js.Json.t) : asset => {
  let assetDict = assetJson |> Js.Json.decodeObject |> Js.Option.getExn;
  {
    id: decodeInt(assetDict, "id"),
    symbol: decodeString(assetDict, "symbol"),
    name: decodeString(assetDict, "name"),
    fiat: decodeBoolean(assetDict, "fiat"),
    route: decodeString(assetDict, "route")
  }
};

let _decodePairJson = (pairJson: Js.Json.t) : pair => {
  let pairDict = pairJson |> Js.Json.decodeObject |> Js.Option.getExn;
  {
    id: decodeInt(pairDict, "id"),
    symbol: decodeString(pairDict, "symbol"),
    base: Js.Dict.get(pairDict, "base") |> Js.Option.getExn |> _decodeAssetJson,
    quote: Js.Dict.get(pairDict, "quote") |> Js.Option.getExn |> _decodeAssetJson,
    route: decodeString(pairDict, "route")
  }
};

let _decodeMarketJson = (marketJson: Js.Json.t) : market => {
  let marketDict = marketJson |> Js.Json.decodeObject |> Js.Option.getExn;
  {
    id: decodeInt(marketDict, "id"),
    exchange: decodeString(marketDict, "exchange"),
    pair: decodeString(marketDict, "pair"),
    active: decodeBoolean(marketDict, "active"),
    route: decodeString(marketDict, "route")
  }
};

let _decodeExchangeJson = (exchange: Js.Json.t) : exchange => {
  let exchangeDict = exchange |> Js.Json.decodeObject |> Js.Option.getExn;
  {
    symbol: decodeString(exchangeDict, "symbol"),
    name: decodeString(exchangeDict, "name"),
    active: decodeBoolean(exchangeDict, "active"),
    route: decodeString(exchangeDict, "route")
  }
};

let fetchAssets = () => fetchArrayFromAPI("https://api.cryptowat.ch/assets", _decodeAssetJson);

let fetchPairs = () => fetchArrayFromAPI("https://api.cryptowat.ch/pairs", _decodePairJson);

let fetchMarkets = () => fetchArrayFromAPI("https://api.cryptowat.ch/markets", _decodeMarketJson);

let fetchExchanges = () =>
  fetchArrayFromAPI("https://api.cryptowat.ch/exchanges", _decodeExchangeJson);