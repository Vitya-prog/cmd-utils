# cmd-utils

A collection of useful command-line utilities for Linux, written in C.

Each utility is self-contained and follows standard Linux development practices.

---

## Utilities

### `hashit`

A simple command-line tool to compute cryptographic hashes of strings or files.  
Supports algorithms provided by the OpenSSL library.

**Usage:**
```bash
hashit [OPTIONS]
```

**Options:**
- `-a, --algorithm <ALGO>` — Hash algorithm to use (`md5`, `sha256`, `sha512`, etc.).
- `-f, --file <PATH>` — Path to a file to hash.
- `-s, --string <TEXT>` — A string to hash.
- `-h, --help` — Display help message and exit.

**Examples:**
```bash
hashit -a sha256 -s "Hello World"
hashit --algorithm md5 --file ./my_document.txt
```

---

### `memstat`

A utility to display memory usage information for the system or for a specific process.  
It reads from `/proc/meminfo` and `/proc/<pid>/status`.

**Usage:**
```bash
memstat [OPTIONS]
```

**Options:**
- `-p, --process <PID>` — Show memory usage for a specific process.
- `-t, --type <TYPE>` — Filter memory info by type (`cached`, `shared`, `anon`, `swap`).
- `-s, --summary` — Show an overall summary of memory usage.
- `-h, --help` — Display help message and exit.

**Examples:**
```bash
memstat -p 1234
memstat -s
memstat -t cached
```

---

## Building and Installation

Use the provided `Makefile` to build and install all utilities.

### Build
```bash
make
```

### Install
```bash
sudo make install
```
Installs binaries to `/usr/local/bin` so they can be used globally.

### Uninstall
```bash
sudo make uninstall
```

### Clean
```bash
make clean
```
Removes compiled binaries from the project directory.

---

## Contributing

Feel free to submit issues or pull requests — all contributions are welcome!

## License

This project is licensed under the MIT License.
