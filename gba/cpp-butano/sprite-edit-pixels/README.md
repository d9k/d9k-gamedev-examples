# Sprite edit pixels

## Build

At first install [butano and required libraries to the system](https://gvaliente.github.io/butano/getting_started_wt.html).

Change `LIBBUTANO` in the [Makefile](./Makefile). Add this path to [.vscode/c_cpp_properties.json](.vscode/c_cpp_properties.json) too, (`configurations -> includePath`).

Then run

```bash
make
```

`sprite-edit-pixels.gba` would be built.

## Run

```bash
mgba-qt sprite-edit-pixels.gba
```

## Credits

Thanks to `Fralacticus` from `gbadev` Discord.