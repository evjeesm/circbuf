# Circbuf

Circular byte buffer for C.  
Is an extension over [vector](https://github.com/evjeesm/vector)

## Implementation details

Efficient fixed size data queue.  
For now it is not Thread Safe.
I am considering to add an extension capabilities,
that will help in making extended TS version relatively easy.


## Supported platforms

| Platforms | CI/CD | COVERAGE |
|---|---|---|
| Linux | ![check-linux](https://github.com/evjeesm/circbuf/actions/workflows/linux.yml/badge.svg) | [![codecov](https://codecov.io/github/evjeesm/circbuf/graph/badge.svg?flag=debian)](https://codecov.io/github/evjeesm/dynarr) |
| Windows | ![check-windows](https://github.com/evjeesm/circbuf/actions/workflows/windows.yml/badge.svg) | [![codecov](https://codecov.io/github/evjeesm/circbuf/graph/badge.svg?flag=windows)](https://codecov.io/github/evjeesm/dynarr) |

[See Full Documentation](https://evjeesm.github.io/circbuf)

## Memory layout

![circbuf-scheme](imgs/circbuf-scheme.svg)
## Dependencies

### Build System

- gcc
- make
- autotools:  
   automake >= 1.11.2  
   autoconf  
   autoconf-archive - install separately (for valgrind support)  
   libtool
- check - testing framework
- valgrind - for memory leak checks
- lcov - for code coverage analizing

### Libraries

- stdlib
- string
- stdbool
- sys/types
- math

### Subprojects:

- [vector](https://github.com/evjeesm/vector)

## Build Process

- Install **Build System** dependencies:

  - On **Debian** / **Ubuntu**:
    - In your fav shell run:
      ```sh
      sudo apt-get install gcc make automake autoconf autoconf-archive libtool \
          check valgrind lcov
      ```
  - On **Windows**:
    - Install [msys2](https://www.msys2.org/) environment.
    - In msys2 shell run:
      ```sh
      pacman -S curl git mingw-w64-ucrt-x86_64-gcc \
          mingw-264-ucrt-x86_64-check \
          autotools autoconf-archive lcov
      ```
      Set up git newline `\n` to `\r\n` convertion (windows style):
      ```sh
      git config --global core.autocrlf input
      ```

- Clone the repository:
  ```sh
  git clone https://github.com/evjeesm/circbuf.git circbuf; cd dynarr;
  git submodule update --init --recursive;
  ```
- Configure project:
  ```sh
  ./autogen.sh && ./configure CFLAGS=<YOUR COMPILATION FLAGS> --prefix=</path/to/install/folder/>
  ```
- Build project: (use -j<threads> option for multithreaded building)
  ```sh
  make
  ```
- Run Tests:
  ```sh
  make check
  make check-valgrind    # optional memory check
  ```
- If no errors occured during _check_ you can safely install library  
  in your desired prefix path that you specified at configure step.  
  Procede to installation:
  ```sh
  make install
  ```

## Usage

Link against `libcircbuf_static.a` or `libcircbuf.so` on **linux**.  
If you on **Windows** platform link to `libcircbuf_static.lib`.

Requires standard math library, so remember to provide `-lm`.  
Requires linking with vector's libraries.

### Minimal Example

```c
#include "circbuf.h"

int main(void)
{
    circbuf_t *circbuf = circbuf_create(.initial_cap = 10);
    int a;
    a = 3;  circbuf_write(&circbuf, sizeof(a), &a);
    a = 10; circbuf_write(&circbuf, sizeof(a), &a);
    a = 42; circbuf_write(&circbuf, sizeof(a), &a);

    /* ... */

    while (circbuf_read_available(circbuf))
    {
        circbuf_read(curcbuf, sizeof(a), &a);
    }

    circbuf_destroy(circbuf);
}
```
