let decodeInt = (dict, key) =>
  Js.Dict.get(dict, key)
  |> Js.Option.andThen([@bs] ((json) => Js.Json.decodeNumber(json)))
  |> Js.Option.getExn
  |> int_of_float;

let decodeString = (dict, key) =>
  Js.Dict.get(dict, key)
  |> Js.Option.andThen([@bs] ((json) => Js.Json.decodeString(json)))
  |> Js.Option.getExn;

let decodeBoolean = (dict, key) =>
  Js.Dict.get(dict, key)
  |> Js.Option.andThen([@bs] ((json) => Js.Json.decodeBoolean(json)))
  |> Js.Option.getExn;

let decodeResultJson = (json: Js.Json.t, decodeJson: Js.Json.t => 'a) : Js.Array.t('a) => {
  let result =
    json
    |> Js.Json.decodeObject
    |> Js.Option.andThen([@bs] ((p) => Js.Dict.get(p, "result")))
    |> Js.Option.getExn;
  let assets = result |> Js.Json.decodeArray |> Js.Option.getExn;
  assets |> Js.Array.map(decodeJson)
};

let fetchArrayFromAPI = (url, decodeSingleElementJson) =>
  Js.Promise.(
    Fetch.fetch(url)
    |> then_(Fetch.Response.json)
    |> then_((json) => decodeResultJson(json, decodeSingleElementJson) |> resolve)
  );