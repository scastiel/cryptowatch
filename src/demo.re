open Crypto;

Js.Promise.(
  fetchMarketDetailsSummary(~exchangeSymbol="bitfinex", ~pairSymbol="btcusd")
  |> then_((asset) => Js.log(asset) |> resolve)
  |> catch((err) => Js.log(err) |> resolve)
);