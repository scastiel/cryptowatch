type market = {
  .
  "id": int, "exchange": string, "pair": string, "active": Js.boolean, "route": string
};

type assetMarkets = {. "base": Js.Array.t(market), "quote": Js.Array.t(market)};

type asset = {
  .
  "id": int,
  "symbol": string,
  "name": string,
  "fiat": Js.boolean,
  "route": option(string),
  "markets": option(assetMarkets)
};

type pair = {
  .
  "id": int,
  "symbol": string,
  "base": asset,
  "quote": asset,
  "route": option(string),
  "markets": option(Js.Array.t(market))
};

type exchange = {. "symbol": string, "name": string, "route": string, "active": Js.boolean};

type detailledAsset = {
  .
  "id": int, "symbol": string, "name": string, "fiat": Js.boolean, "markets": assetMarkets
};

type change = {. "percentage": float, "absolute": float};

type price = {. "last": float, "high": float, "low": float, "change": change};

type marketSummary = {. "price": price, "volume": float};

type marketPrice = {. "price": float};