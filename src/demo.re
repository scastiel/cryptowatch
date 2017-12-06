open Crypto;

Js.Promise.(
  fetchMarketDetailsSummary("bitfinex", "btcusd")
  |> then_((asset) => Js.log(asset) |> resolve)
  |> catch((err) => Js.log(err) |> resolve)
);