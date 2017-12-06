let makeUrl = (baseUrl, args) => {
  let rec join = (list) =>
    switch list {
    | [] => ""
    | [fst] => fst
    | [fst, ...rst] => fst ++ "/" ++ join(rst)
    };
  baseUrl ++ join(args)
};

let decodeInt = (dict, key) =>
  Js.Dict.get(dict, key)
  |> Js.Option.andThen([@bs] ((json) => Js.Json.decodeNumber(json)))
  |> Js.Option.getExn
  |> int_of_float;

let decodeFloat = (dict, key) =>
  Js.Dict.get(dict, key)
  |> Js.Option.andThen([@bs] ((json) => Js.Json.decodeNumber(json)))
  |> Js.Option.getExn;

let decodeString = (dict, key) =>
  Js.Dict.get(dict, key)
  |> Js.Option.andThen([@bs] ((json) => Js.Json.decodeString(json)))
  |> Js.Option.getExn;

let decodeBoolean = (dict, key) =>
  Js.Dict.get(dict, key)
  |> Js.Option.andThen([@bs] ((json) => Js.Json.decodeBoolean(json)))
  |> Js.Option.getExn;

let decodeArray = (dict, key, decodeJson) =>
  Js.Dict.get(dict, key)
  |> Js.Option.getExn
  |> Js.Json.decodeArray
  |> Js.Option.getExn
  |> Js.Array.map((json) => json |> Js.Json.decodeObject |> Js.Option.getExn |> decodeJson);

let decodeObject = (dict, key, decodeJson) =>
  Js.Dict.get(dict, key)
  |> Js.Option.getExn
  |> Js.Json.decodeObject
  |> Js.Option.getExn
  |> decodeJson;

let decodeResultArrayJson =
    (json: Js.Json.t, decodeJson: Js.Dict.t(Js.Json.t) => 'a)
    : Js.Array.t('a) => {
  let result =
    json
    |> Js.Json.decodeObject
    |> Js.Option.andThen([@bs] ((p) => Js.Dict.get(p, "result")))
    |> Js.Option.getExn;
  let assets = result |> Js.Json.decodeArray |> Js.Option.getExn;
  assets |> Js.Array.map((json) => json |> Js.Json.decodeObject |> Js.Option.getExn |> decodeJson)
};

let decodeResultSingleElementJson = (json: Js.Json.t, decodeJson: Js.Dict.t(Js.Json.t) => 'a) : 'a => {
  let result =
    json
    |> Js.Json.decodeObject
    |> Js.Option.andThen([@bs] ((p) => Js.Dict.get(p, "result")))
    |> Js.Option.getExn;
  let elementJson = result |> Js.Json.decodeObject |> Js.Option.getExn;
  decodeJson(elementJson)
};

let fetchArrayFromAPI = (url, decodeSingleElementJson) =>
  Js.Promise.(
    Fetch.fetch(url)
    |> then_(Fetch.Response.json)
    |> then_((json) => decodeResultArrayJson(json, decodeSingleElementJson) |> resolve)
  );

let fetchSingleElementFromAPI = (url, decodeSingleElementJson) =>
  Js.Promise.(
    Fetch.fetch(url)
    |> then_(Fetch.Response.json)
    |> then_((json) => decodeResultSingleElementJson(json, decodeSingleElementJson) |> resolve)
  );