open Crypto;

Js.Promise.(
  fetchAssets()
  |> then_((asset) => Js.log(asset) |> resolve)
  |> catch((err) => Js.log(err) |> resolve)
);