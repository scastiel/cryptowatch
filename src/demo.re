open Cryptowatch;

Js.Promise.(
  fetchPairs()
  |> then_((assets) => Js.log(assets) |> resolve)
  |> catch((err) => Js.log(err) |> resolve)
);