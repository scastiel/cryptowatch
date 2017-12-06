open Types;

let fetchAssets: unit => Js.Promise.t(Js.Array.t(asset));

let fetchPairs: unit => Js.Promise.t(Js.Array.t(pair));

let fetchMarkets: unit => Js.Promise.t(Js.Array.t(market));

let fetchExchanges: unit => Js.Promise.t(Js.Array.t(exchange));

let fetchAssetDetails: (~assetId: int) => Js.Promise.t(asset);

let fetchPairDetails: (~pairId: int) => Js.Promise.t(pair);

let fetchMarketDetailsSummary:
  (~exchangeSymbol: string, ~pairSymbol: string) => Js.Promise.t(marketSummary);

let fetchMarketDetailsPrice: (string, string) => Js.Promise.t(marketPrice);