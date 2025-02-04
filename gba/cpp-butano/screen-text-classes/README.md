# Screen text classes

![screenshot](./screenshot.png)

Forked from [Butano 18.6.0](https://github.com/GValiente/butano/tree/18.6.0/examples/text).

UI object-oriented abstractions over `text_generator.generate()` including `ScrollableBlock`, `RowsComposer`, `CaptionValuePair`, `Title` classes.

## Build

At first install [butano and required libraries to the system](https://gvaliente.github.io/butano/getting_started_wt.html).

Change `LIBBUTANO` in the [Makefile](./Makefile). Add this path to [.vscode/c_cpp_properties.json](.vscode/c_cpp_properties.json) too, (`configurations -> includePath`).

Then run

```bash
make
```

`screen-text-classes.gba` would be built.

## Run

```bash
mgba-qt screen-text-classes.gba
```

or just

```bash
make start
```

## List available make actions

```
make list
```
