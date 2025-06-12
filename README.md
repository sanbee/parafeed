# parafeed [![CI](https://github.com/sanbee/parafeed/actions/workflows/parafeed_CI.yml/badge.svg)](https://github.com/sanbee/parafeed/actions/workflows/parafeed_CI.yaml)

A library for an embedded user interface designed for ease of use for the programmer as well as the end-user. It embeds a shell in the application which can be configured for an interactive session or a non-interactive command line interface. The shell can be customized via command-line arguments or via environment variables. The user can access the shell via simple `arg=value1,value2,...` command-line arguments (useful when writing pipelines). In an interactive session, the list of parameters are displayed as a list of `arg=<Value(s)>` on the screen (see [examples](https://github.com/sanbee/parafeed/blob/wiki/UserDoc.md#example)) and the user can `set/reset/save/load/edit` parameter values (see [User Commands](https://github.com/sanbee/parafeed/blob/wiki/UserDoc.md#user-commands)). Command line editing, min-matching, TABBED completion, parameter hiding/exposing based on the value of other parameter is also supported (see the [full user manual](https://github.com/sanbee/parafeed/blob/wiki/UserDoc.md)). 


### Build System(s)
_parafeed_ project can be built with the `cmake` tool.  Alternatively, it can also be built directly using the hand-written `makefiles`.

CMake-based build system requires `cmake` version 3 or later.  To build, execute the `cmake` command in the top-most directory.  This will build the required `makefiles`.  Issuing the `make` command, also in the root directory, should build the _parafeed_ package.

While the hand-written `makefiles` have been tested with `make` version 3 only, they do not depend on a specific version of `make`.  These `makefiles` are in the `code/nocmake_makefiles` directory. Using the command `cd code; make -f nocmake_makefiles/makefile` should build the _parafeed_ project.

Successfully building and linking the test program `code/tstcpp` ensures that all libraries required by client codes are ready.  For now, copy all `code/libparafeed.a`, `code/*.h` and `code/calc/*.h` files to the required location for linking your application.

The CI system triggers a build-and-test on changes to the `master` branch. 

[![CI](https://github.com/sanbee/parafeed/actions/workflows/parafeed_CI.yml/badge.svg)](https://github.com/sanbee/parafeed/actions/workflows/parafeed_CI.yml)

### Interfaces
The library `libparafeed.a` contains .o files from three independent libraries: `libsh.a`, `libshccl.a` and `libcalc.a`.  These can be built and linked-to separately as well in the order `libshccl.a libsh.a libcalc.a`.  For use in applications written in C and C++, `libparafeed.a` is sufficient.

While at this point the package is primarily meant for use in applications written in the C++ and C languages, code for FORTRAN interface exists and was used in the past.  For use with FORTRAN, `libshfcl.a` is required instead of `libshccl.a`.  However, this interface has not been used for a long time and may be in disrepair. If you do find the need for the FORTRAN interface and are not able to build `libshfcl.a`, please contact the authors.

Note that the C and FORTRAN interfaces have limitations.  In particular, [context based exposing/hiding](https://github.com/sanbee/parafeed/blob/wiki/UserDoc.md#parameter-unwinding-context-sensitive-parameter-hidingexposing) of parameters is available only via the C++ interface.  C++ interface can however be used in most C programs if they can be complied with a C++ compiler.

A more detailed document describing the API is under preparation.  Till then, ask for help from the authors.
