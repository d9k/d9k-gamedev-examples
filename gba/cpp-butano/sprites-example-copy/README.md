# Sprites example copy

Copied from [Butano 18.1.0](https://github.com/GValiente/butano/tree/18.1.0/examples/sprites).

## Build

At first install [butano and required libraries to the system](https://gvaliente.github.io/butano/getting_started_wt.html).

Change `LIBBUTANO` in the [Makefile](./Makefile). Add this path to [.vscode/c_cpp_properties.json](.vscode/c_cpp_properties.json) too, (`configurations -> includePath`).

Then run

```bash
make
```

`sprites-example-copy.gba` would be built.

## Run

```bash
mgba-qt sprites-example-copy.gba
```

