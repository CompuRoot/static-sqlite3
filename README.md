```
     ____     ___    _       _   _            _____
    / ___|   / _ \  | |     (_) | |_    ___  |___ /
    \___ \  | | | | | |     | | | __|  / _ \   |_ \
     ___) | | |_| | | |___  | | | |_  |  __/  ___) |
    |____/   \__\_\ |_____| |_|  \__|  \___| |____/
                                                    
```
# static-sqlite3

Compile a statically linked `sqlite3` for amd64 platforms due to abcense of statically compiled `sqlite3` program for x86_64 Linux platforms on official site.

## Compilation Requirements
Docker required (using official Linux distribution of alpine:latest)

## Running Requirements

- Linux
- compatible with classic `sh` shells, such as bash,dash,ash,etc

- Optional:
  - OpenSSH 8.0+ (to use `ssh-keygen` to verify compiled files in [release][1] section)
  - `git` (to pull conveniently this repository)
  - `gpg` (to verify signed tar.gz and zip archives)

## Compiling

```bash
git clone https://github.com/CompuRoot/static-sqlite3.git
cd static-sqlite3
./build_static_sqlite.sh
```
Compiled file will be placed in your local `release` directory.

## Pre-compiled binary

You can also download already compiled, `sqlite3` program from [releases][1].<br>
To check integrity and authenticity of pre-compiled program, - download both, `sqlite3` and its signature `sqlite3.sig`,
and run following command in a download directory:
```
<sqlite3 ssh-keygen -Y check-novalidate -n 'https://github.com/compuroot.file' -s sqlite3.sig
```
Also, `tar.gz` and `zip` archives in release section are signed with GPG

Starting from SQLite version 3.45.3, compiled versions of releases will be represented as `XX.XX.XX_ZZ` where `XX.XX.XX` will reflect actuall SQLite versions while `ZZ` will represent version of this building tool.

## Customization

- You can change `sqlite3` version to compile by edit file `build_static_sqlite.sh` . You need to supply direct link to official `sqlite3` download link in variable: `SQLITE_ZIP_URL`
- By default compiled `sqlite3` stripped and packed with `upx` compressor to reduce file size. If you don’t want to have compressed version, keep variable `SQLite_compressor` empty.
- You can change compile-time options by editing `docker/Build.sh` file before running `build_static_sqlite.sh`. As of now, `Build.sh` following closely to official recommendation with compile-time options.

## Motivation

Make portable `sqlite3` program that can run without dependencies on any x86_64 Linux operation systems.


[1]: https://github.com/CompuRoot/static-sqlite3/releases
