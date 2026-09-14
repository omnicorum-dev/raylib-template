# raylib-lua

A small C host application that embeds Lua 5.4 and exposes a hand-written
[raylib](https://www.raylib.com/) binding layer, so the actual game is
written in Lua while raylib and Lua itself are built from vendored source.
There's no package manager step and no submodules to init — `vendor/raylib`
and `vendor/lua` are committed source trees that the Makefile compiles for
you.

## Requirements

You need a C compiler and `make`. Nothing else is fetched at build time —
raylib and Lua are built from the source already in `vendor/`.

**macOS**

Install the Xcode Command Line Tools if you haven't already:

```
xcode-select --install
```

That provides `cc`/`make` and the OpenGL, Cocoa, IOKit and CoreVideo
frameworks the linker needs. Nothing else to install.

**Linux**

A compiler, `make`, and the X11/OpenGL development headers:

```
sudo apt install build-essential libgl1-mesa-dev libx11-dev
```

(package names above are for Debian/Ubuntu; use your distro's equivalents)

**Windows**

A MinGW-w64 toolchain (`gcc`/`make`) on your `PATH`. `opengl32`, `gdi32` and
`winmm` ship with MinGW, so there's nothing extra to fetch.

## Building & running

```
make        # builds vendor/raylib and vendor/lua from source, then build/main(.exe)
make run    # build (if needed) and launch it
```

The executable resolves its own location at startup and `chdir`s back to
the project root, so it runs correctly whether you launch it as
`./build/main`, via `make run`, or from a file browser — it will always
find `game/main.lua` and `assets/` relative to the project root, not
whatever directory you happened to launch it from.

Other Makefile targets:

```
make clean  # removes build/ and all compiled raylib/Lua objects and libs
```

## Project layout

```
src/
  main.c               - entry point: creates the Lua state, registers the
                          bindings, then loads and runs game/main.lua
  raylib_bindings.h    - the raylib <-> Lua binding layer (this is where
                          new raylib functions get exposed to Lua)

game/
  main.lua             - game entry point: window/audio setup and the main
                          loop live here
  player.lua           - example module (Player class) required from
                          main.lua, showing how to split game code across
                          files

assets/
  netskie.png          - example texture
  shaders/tint.fs       - example fragment shader

types/
  c_api.lua            - LuaLS (Lua Language Server) type annotations for
                          every binding, kept in sync with
                          src/raylib_bindings.h

vendor/
  raylib/              - raylib source, built by the Makefile
  lua/                 - Lua 5.4 source, built by the Makefile
```

## Editor setup

`.luarc.json` points [LuaLS](https://github.com/LuaLS/lua-language-server)
at `types/c_api.lua`, so any editor with the Lua Language Server extension
(e.g. VS Code's "Lua" extension by sumneko/LuaLS) gets autocomplete, hover
docs, and type checking for every raylib binding while editing files under
`game/`.

## Adding new bindings

Every raylib function exposed to Lua lives in `src/raylib_bindings.h` as a
`lua_XxxYyy` C function, registered as a bare global near the bottom of the
file in `pushLuaFunctions`. When you add or change a binding, mirror the
change in `types/c_api.lua` so editor autocomplete stays accurate.
