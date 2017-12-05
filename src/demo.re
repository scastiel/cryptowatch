open Cryptowatch;

Js.Promise.(
  fetchAssetDetails("btc")
  |> then_((asset) => Js.log(asset) |> resolve)
  |> catch((err) => Js.log(err) |> resolve)
);