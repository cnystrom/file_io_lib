# file_io_lib: home directory, file and path helpers

Version 1.1 (2026). Split from NewIO 0.12, https://github.com/cnystrom/newio

## Versions

- **1.0:** original code as published in NewIO 0.12 (tag `v1.0`).
- **1.1:** this release, with the fixes listed below. See `CHANGES.md`.

file_io_lib holds NewIO's small file helpers: find the home directory,
test whether a file exists, create a directory if it is missing, take a
basename, get a file size, and copy or append fixed-size paths safely.

## API

From `file_io_lib.h`:

| Function | What it does |
|---|---|
| `int get_home_dir(char *s)` | Copies `$HOME` into `s` (up to `PATH_SIZE`, 256). On Windows it tries `HOME`, `USERPROFILE`, then the profile folder. If `HOME` is unset (Unix), uses the password-file home directory, or an empty string. Returns the length. |
| `int file_exists(const char *s)` | `TRUE` if `s` is a regular file. Directories return `FALSE`. |
| `void check_dir(const char *s)` | Creates directory `s` (mode 0755) if it doesn't exist. |
| `char *niobasename(char *s)` | Pointer to the part after the last `/` (also `\\` on Windows). |
| `long file_size(const char *s)` | Size of a regular file, or 0. |
| `size_t pathcpy(path dst, const char_ptr src)` | `strlcpy` into a `path` (`char[256]`). |
| `size_t pathcat(path dst, const char_ptr src)` | `strlcat` onto a `path`. |

`file_io_lib_version.h` defines `FILE_IO_LIB_VERSION` ("1.1") and
`FILE_IO_LIB_ORIGIN`.

## Dependencies

- **str_lib** (sibling project) for `strlcpy`/`strlcat`.
- **log_lib** (sibling project) for error messages.

The `Makefile` looks for them in `../str_lib` and `../log_lib` and builds
them if needed. Point elsewhere with `make STR_LIB_DIR=... LOG_LIB_DIR=...`.
Nothing from them is copied into this project. Link order:
`file_io_lib.a str_lib.a log_lib.a`.

## Build

Check out the sibling projects next to this one, then:

```sh
make            # builds file_io_lib.a (UNIX is defined by the Makefile)
make test       # builds the siblings if needed, then runs test_file_io_lib
make CC=clang test
make clean
```

When unpacked from the release tarballs, the directory names include the
version, so give the sibling paths:

```sh
make STR_LIB_DIR=../str_lib-1.1 LOG_LIB_DIR=../log_lib-1.1 test
```

The code needs `-DUNIX` on Linux, BSD and macOS, as in NewIO. The `Makefile`
passes it.

## License

LGPL 2.1 or later, the same as NewIO. See `LICENSE` (GNU LGPL 2.1). `COPYING`
holds the GNU GPL 2, as in NewIO. Copyright Chris Nystrom; see the header of
each file for its years.

## Origin

Split from NewIO 0.12, https://github.com/cnystrom/newio (tag `newio-0.12`,
commit `ce54c5b`), directory `common/file_io_lib/`. The library code is Chris
Nystrom's original, copied unchanged except for the lines listed under
"Changes from NewIO" below.

Contact: Chris Nystrom <cnystrom@gmail.com>

## Changes from NewIO

- `misc_io.c:42`: `#include "../nio_log/nio_log.h"` became
  `#include "log_lib.h"`. The `nio_log_print` calls are unchanged and work
  through log_lib's compatibility names.
- `misc_io.c:37-39` and `:129-132`: bug fix, see "Fixed in 1.1".
- Removed two old personal contact lines (a web-log URL and a chat handle) from the file headers, at Chris's request (2026). The rest of each header and the authorship are unchanged, so header-area line numbers are 2 lower than in NewIO.
- `Makefile:34-36`: `-I.. -I../str_lib` became `-I. -I$(STR_LIB_DIR)
  -I$(LOG_LIB_DIR)`.
- Added a copy of NewIO's `common/nio_types.h` (unchanged),
  `file_io_lib_version.h`, `test_file_io_lib.c`, the sibling and `make test`
  rules at the end of the `Makefile`, this README, `LICENSE` and `COPYING`.
- Not copied: `archive.sh`.
- Left as is: gcc and clang warn that `i` is set but not used
  (`misc_io.c:94`). It's harmless.

## Fixed in 1.1

- **`get_home_dir()` crashed when `HOME` was unset** (`misc_io.c:37-39`,
  `:129-132`). It logged a message and then called `strlen(NULL)`. It now
  falls back to the home directory in the password file
  (`getpwuid(getuid())`), or to an empty string, and returns that length.
  Tested in `test_file_io_lib.c`.

## Known issues

Kept as in NewIO 0.12:

- `misc_io.c:79` (Windows only): `GetWindowsDirectory(windowsdir,
  sizeof(windowsdir))` passes the size of a pointer, not of the buffer.
  `MAX_PATH` was meant.
- `misc_io.c:235` (Windows only): `file_size()` adds `c:/cygwin` in front of
  the path.
- The Windows code paths have not been built or tested in this split.
