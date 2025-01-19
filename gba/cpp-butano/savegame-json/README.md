# Savegame JSON example

Forked from [Butano 18.6.0](https://github.com/GValiente/butano/tree/18.6.0/examples/text).

Example data in [include/palestinian_movies_cut_json.h](./include/palestinian_movies_cut_json.h) is from the https://www.kaggle.com/datasets/sondosaabed/palestinian-movies-json-dataset (no political reasons. This dataset just had enough upvotes to appear at first page of my kaggle search).

Of course there is no need to store whole movies data in save game file. I implemented it for reason of training only (planning to store generated world in my future game).

Used [SAX event-based parser of RapidJSON library](https://rapidjson.org/md_doc_sax.html) library because of small memory footprint (no intermediate structures).

## Build

At first install [butano and required libraries to the system](https://gvaliente.github.io/butano/getting_started_wt.html).

Change `LIBBUTANO` in the [Makefile](./Makefile). Add this path to [.vscode/c_cpp_properties.json](.vscode/c_cpp_properties.json) too, (`configurations -> includePath`).

Then run

```bash
make
```

`pseudo-random.gba` would be built.

## Run

```bash
mgba-qt savegame-json.gba
```

or just

```bash
make start
```

## Regenerate pre-defined pseudo-random array

```bash
generate-pseudo-random-array
```

## List available make actions

```
make list
```
