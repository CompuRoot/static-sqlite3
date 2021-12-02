# static-sqlite3

Compile a statically linked `sqlite3` for amd64 platforms due to absents of statically compiled `sqlite3` program for amd64 Linux platforms on official site.

## Compilation Requirements
Docker required (using official Linux distribution of alpine:latest)

## Running Requirements

Linux

## Compiling

```bash
git clone https://github.com/CompuRoot/static-sqlite3.git
cd static-sqlite3
./build_static_sqlite.sh
```
Compiled file will be placed in `release` directory.

You can also download already compiled, `sqlite3` program from [release][1] directory.

## Customization

- You can change `sqlite3` version to compile by edit file `build_static_sqlite.sh` . You need to supply direct link to official `sqlite3` download link in variable: `SQLITE_ZIP_URL`
- By default compiled `sqlite3` striped and packed with `upx` compressor. If you won't to have compressed version, keep variable `SQLite_compressor` empty.

## Motivation
Make portable `sqlite3` program that can run without dependencies.


[1]: https://github.com/CompuRoot/static-sqlite3/releases
