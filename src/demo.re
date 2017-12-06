open Crypto;

Js.Promise.(
  fetchMarketDetailsPrice("bitfinex", "btcusd")
  |> then_((asset) => Js.log(asset) |> resolve)
  |> catch((err) => Js.log(err) |> resolve)
);