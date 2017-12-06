open Utils;

type market = {
  id: int,
  exchange: string,
  pair: string,
  active: Js.boolean,
  route: string
};

type assetMarkets = {
  base: Js.Array.t(market),
  quote: Js.Array.t(market)
};

type asset = {
  id: int,
  symbol: string,
  name: string,
  fiat: Js.boolean,
  route: option(string),
  markets: option(assetMarkets)
};

type pair = {
  id: int,
  symbol: string,
  base: asset,
  quote: asset,
  route: option(string),
  markets: option(Js.Array.t(market))
};

type exchange = {
  symbol: string,
  name: string,
  route: string,
  active: Js.boolean
};

type detailledAsset = {
  id: int,
  symbol: string,
  name: string,
  fiat: Js.boolean,
  markets: assetMarkets
};

type change = {
  percentage: float,
  absolute: float
};

type price = {
  last: float,
  high: float,
  low: float,
  change
};

type marketSummary = {
  price,
  volume: float
};

type marketPrice = {price: float};

let _decodeAssetJson = (assetDict: Js.Dict.t(Js.Json.t)) : asset => {
  id: decodeInt(assetDict, "id"),
  symbol: decodeString(assetDict, "symbol"),
  name: decodeString(assetDict, "name"),
  fiat: decodeBoolean(assetDict, "fiat"),
  route: Some(decodeString(assetDict, "route")),
  markets: None
};

let _decodePairJson = (pairDict) : pair => {
  id: decodeInt(pairDict, "id"),
  symbol: decodeString(pairDict, "symbol"),
  base: decodeObject(pairDict, "base", _decodeAssetJson),
  quote: decodeObject(pairDict, "quote", _decodeAssetJson),
  route: Some(decodeString(pairDict, "route")),
  markets: None
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

let _decodeDetailledAssetJson = (assetDict: Js.Dict.t(Js.Json.t)) : asset => {
  id: decodeInt(assetDict, "id"),
  symbol: decodeString(assetDict, "symbol"),
  name: decodeString(assetDict, "name"),
  fiat: decodeBoolean(assetDict, "fiat"),
  route: None,
  markets:
    Some(
      decodeObject(
        assetDict,
        "markets",
        (marketsDict) => {
          base: decodeArray(marketsDict, "base", _decodeMarketJson),
          quote: decodeArray(marketsDict, "quote", _decodeMarketJson)
        }
      )
    )
};

let _decodeDetailledPairJson = (pairDict: Js.Dict.t(Js.Json.t)) : pair => {
  id: decodeInt(pairDict, "id"),
  symbol: decodeString(pairDict, "symbol"),
  base: decodeObject(pairDict, "base", _decodeAssetJson),
  quote: decodeObject(pairDict, "quote", _decodeAssetJson),
  route: None,
  markets: Some(decodeArray(pairDict, "markets", _decodeMarketJson))
};

let _decodeDetailledMarketPrice = (priceDict: Js.Dict.t(Js.Json.t)) : marketPrice => {
  price: decodeFloat(priceDict, "price")
};

let _decodeDetailledMarketSummary = (summaryDict: Js.Dict.t(Js.Json.t)) : marketSummary => {
  price:
    decodeObject(
      summaryDict,
      "price",
      (priceDict) => {
        last: decodeFloat(priceDict, "last"),
        high: decodeFloat(priceDict, "high"),
        low: decodeFloat(priceDict, "low"),
        change:
          decodeObject(
            priceDict,
            "change",
            (changeDict) => {
              percentage: decodeFloat(changeDict, "percentage"),
              absolute: decodeFloat(changeDict, "absolute")
            }
          )
      }
    ),
  volume: decodeFloat(summaryDict, "volume")
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

let fetchMarketDetailsSummary = (exchangeSymbol, pairSymbol) =>
  fetchSingleElementFromAPI(
    "https://api.cryptowat.ch/markets/" ++ exchangeSymbol ++ "/" ++ pairSymbol ++ "/summary",
    _decodeDetailledMarketSummary
  );

let fetchMarketDetailsPrice = (exchangeSymbol, pairSymbol) =>
  fetchSingleElementFromAPI(
    "https://api.cryptowat.ch/markets/" ++ exchangeSymbol ++ "/" ++ pairSymbol ++ "/price",
    _decodeDetailledMarketPrice
  );