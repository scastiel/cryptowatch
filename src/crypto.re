open Utils;

include Types;

let baseUrl = "https://api.cryptowat.ch/";

let _decodeAssetJson = (assetDict: Js.Dict.t(Js.Json.t)) : asset => {
  "id": decodeInt(assetDict, "id"),
  "symbol": decodeString(assetDict, "symbol"),
  "name": decodeString(assetDict, "name"),
  "fiat": decodeBoolean(assetDict, "fiat"),
  "route": Some(decodeString(assetDict, "route")),
  "markets": None
};

let _decodePairJson = (pairDict) : pair => {
  "id": decodeInt(pairDict, "id"),
  "symbol": decodeString(pairDict, "symbol"),
  "base": decodeObject(pairDict, "base", _decodeAssetJson),
  "quote": decodeObject(pairDict, "quote", _decodeAssetJson),
  "route": Some(decodeString(pairDict, "route")),
  "markets": None
};

let _decodeMarketJson = (marketDict) : market => {
  "id": decodeInt(marketDict, "id"),
  "exchange": decodeString(marketDict, "exchange"),
  "pair": decodeString(marketDict, "pair"),
  "active": decodeBoolean(marketDict, "active"),
  "route": decodeString(marketDict, "route")
};

let _decodeExchangeJson = (exchangeDict) : exchange => {
  "symbol": decodeString(exchangeDict, "symbol"),
  "name": decodeString(exchangeDict, "name"),
  "active": decodeBoolean(exchangeDict, "active"),
  "route": decodeString(exchangeDict, "route")
};

let _decodeDetailledAssetJson = (assetDict: Js.Dict.t(Js.Json.t)) : asset => {
  "id": decodeInt(assetDict, "id"),
  "symbol": decodeString(assetDict, "symbol"),
  "name": decodeString(assetDict, "name"),
  "fiat": decodeBoolean(assetDict, "fiat"),
  "route": None,
  "markets":
    Some(
      decodeObject(
        assetDict,
        "markets",
        (marketsDict) => {
          "base": decodeArray(marketsDict, "base", _decodeMarketJson),
          "quote": decodeArray(marketsDict, "quote", _decodeMarketJson)
        }
      )
    )
};

let _decodeDetailledPairJson = (pairDict: Js.Dict.t(Js.Json.t)) : pair => {
  "id": decodeInt(pairDict, "id"),
  "symbol": decodeString(pairDict, "symbol"),
  "base": decodeObject(pairDict, "base", _decodeAssetJson),
  "quote": decodeObject(pairDict, "quote", _decodeAssetJson),
  "route": None,
  "markets": Some(decodeArray(pairDict, "markets", _decodeMarketJson))
};

let _decodeDetailledMarketPrice = (priceDict: Js.Dict.t(Js.Json.t)) : marketPrice => {
  "price": decodeFloat(priceDict, "price")
};

let _decodeDetailledMarketSummary = (summaryDict: Js.Dict.t(Js.Json.t)) : marketSummary => {
  "price":
    decodeObject(
      summaryDict,
      "price",
      (priceDict) => {
        "last": decodeFloat(priceDict, "last"),
        "high": decodeFloat(priceDict, "high"),
        "low": decodeFloat(priceDict, "low"),
        "change":
          decodeObject(
            priceDict,
            "change",
            (changeDict) => {
              "percentage": decodeFloat(changeDict, "percentage"),
              "absolute": decodeFloat(changeDict, "absolute")
            }
          )
      }
    ),
  "volume": decodeFloat(summaryDict, "volume")
};

let fetchAssets = () => fetchArrayFromAPI(makeUrl(baseUrl, ["assets"]), _decodeAssetJson);

let fetchPairs = () => fetchArrayFromAPI(makeUrl(baseUrl, ["pairs"]), _decodePairJson);

let fetchMarkets = () => fetchArrayFromAPI(makeUrl(baseUrl, ["markets"]), _decodeMarketJson);

let fetchExchanges = () => fetchArrayFromAPI(makeUrl(baseUrl, ["exchanges"]), _decodeExchangeJson);

let fetchAssetDetails = (~assetId) =>
  fetchSingleElementFromAPI(
    makeUrl(baseUrl, ["assets", string_of_int(assetId)]),
    _decodeDetailledAssetJson
  );

let fetchPairDetails = (~pairId) =>
  fetchSingleElementFromAPI(
    makeUrl(baseUrl, ["pairs", string_of_int(pairId)]),
    _decodeDetailledPairJson
  );

let fetchMarketDetailsSummary = (~exchangeSymbol, ~pairSymbol) =>
  fetchSingleElementFromAPI(
    makeUrl(baseUrl, ["markets", exchangeSymbol, pairSymbol, "summary"]),
    _decodeDetailledMarketSummary
  );

let fetchMarketDetailsPrice = (exchangeSymbol, pairSymbol) =>
  fetchSingleElementFromAPI(
    makeUrl(baseUrl, ["markets", exchangeSymbol, pairSymbol, "price"]),
    _decodeDetailledMarketPrice
  );