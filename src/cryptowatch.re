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

type assetMarkets = {
  base: Js.Array.t(market),
  quote: Js.Array.t(market)
};

type detailledAsset = {
  id: int,
  symbol: string,
  name: string,
  fiat: Js.boolean,
  markets: assetMarkets
};

type detailledPair = {
  id: int,
  symbol: string,
  base: asset,
  quote: asset,
  markets: Js.Array.t(market)
};

let _decodeAssetJson = (assetDict: Js.Dict.t(Js.Json.t)) : asset => {
  id: decodeInt(assetDict, "id"),
  symbol: decodeString(assetDict, "symbol"),
  name: decodeString(assetDict, "name"),
  fiat: decodeBoolean(assetDict, "fiat"),
  route: decodeString(assetDict, "route")
};

let _decodePairJson = (pairDict) : pair => {
  id: decodeInt(pairDict, "id"),
  symbol: decodeString(pairDict, "symbol"),
  base:
    Js.Dict.get(pairDict, "base")
    |> Js.Option.getExn
    |> Js.Json.decodeObject
    |> Js.Option.getExn
    |> _decodeAssetJson,
  quote:
    Js.Dict.get(pairDict, "quote")
    |> Js.Option.getExn
    |> Js.Json.decodeObject
    |> Js.Option.getExn
    |> _decodeAssetJson,
  route: decodeString(pairDict, "route")
};

let _decodeMarketJson = (marketDict) : market => {
  id: decodeInt(marketDict, "id"),
  exchange: decodeString(marketDict, "exchange"),
  pair: decodeString(marketDict, "pair"),
  active: decodeBoolean(marketDict, "active"),
  route: decodeString(marketDict, "route")
};

let _decodeExchangeJson = (exchangeDict) : exchange => {
  symbol: decodeString(exchangeDict, "symbol"),
  name: decodeString(exchangeDict, "name"),
  active: decodeBoolean(exchangeDict, "active"),
  route: decodeString(exchangeDict, "route")
};

let _decodeDetailledAssetJson = (assetDict: Js.Dict.t(Js.Json.t)) : detailledAsset => {
  let marketsObj =
    Js.Dict.get(assetDict, "markets")
    |> Js.Option.getExn
    |> Js.Json.decodeObject
    |> Js.Option.getExn;
  {
    id: decodeInt(assetDict, "id"),
    symbol: decodeString(assetDict, "symbol"),
    name: decodeString(assetDict, "name"),
    fiat: decodeBoolean(assetDict, "fiat"),
    markets: {
      base: decodeArray(marketsObj, "base", _decodeMarketJson),
      quote: decodeArray(marketsObj, "quote", _decodeMarketJson)
    }
  }
};

let _decodeDetailledPairJson = (pairDict: Js.Dict.t(Js.Json.t)) : detailledPair => {
  id: decodeInt(pairDict, "id"),
  symbol: decodeString(pairDict, "symbol"),
  base:
    Js.Dict.get(pairDict, "base")
    |> Js.Option.getExn
    |> Js.Json.decodeObject
    |> Js.Option.getExn
    |> _decodeAssetJson,
  quote:
    Js.Dict.get(pairDict, "quote")
    |> Js.Option.getExn
    |> Js.Json.decodeObject
    |> Js.Option.getExn
    |> _decodeAssetJson,
  markets: decodeArray(pairDict, "markets", _decodeMarketJson)
};

let fetchAssets = () => fetchArrayFromAPI("https://api.cryptowat.ch/assets", _decodeAssetJson);

let fetchPairs = () => fetchArrayFromAPI("https://api.cryptowat.ch/pairs", _decodePairJson);

let fetchMarkets = () => fetchArrayFromAPI("https://api.cryptowat.ch/markets", _decodeMarketJson);

let fetchExchanges = () =>
  fetchArrayFromAPI("https://api.cryptowat.ch/exchanges", _decodeExchangeJson);

let fetchAssetDetails = (assetId) =>
  fetchSingleElementFromAPI(
    "https://api.cryptowat.ch/assets/" ++ assetId,
    _decodeDetailledAssetJson
  );

let fetchPairDetails = (pairId) =>
  fetchSingleElementFromAPI("https://api.cryptowat.ch/pairs/" ++ pairId, _decodeDetailledPairJson);