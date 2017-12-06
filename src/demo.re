open Crypto;

Js.Promise.(
  fetchPairDetails("btcusd")
  |> then_((asset) => Js.log(asset) |> resolve)
  |> catch((err) => Js.log(err) |> resolve)
);