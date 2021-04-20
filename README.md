# parafeed
A library for an embedded user interface designed for ease of use for the programmer as well as the end-user. It embeds a shell in the application which can be configured for an interactive session or a non-interactive command line interface. The shell can be customized via command-line arguments or via environment variables. The user can access the shell via simple 'arg=value1,value2,...' command-line arguments (useful when writing pipelines). In an interactive session, the list of parameters are displayed as a list of 'arg=<Value(s)>' on the screen (see [examples](https://github.com/sanbee/parafeed/blob/wiki/UserDoc.md#example)) and the user can set/reset/save/load/edit parameter values (see [User Commands](https://github.com/sanbee/parafeed/blob/wiki/UserDoc.md#user-commands)). Command line editing, min-matching, TABBED completion, parameter hiding/exposing based on the value of other parameter is also supported (see the [full user manual](https://github.com/sanbee/parafeed/blob/wiki/UserDoc.md)). 

For full documentation, go [here](https://github.com/sanbee/parafeed/blob/wiki/UserDoc.md)

### Build System(s)
_parafeed_ project can be built with the `cmake` tool.  Alternatively, it can also be built directly using the hand-written `makefiles`.

CMake-based build system requires `cmake` version 3 or later.  To build, execute the `cmake` command in the top-most directory.  This will build the required `makefiles`.  Issuing the `make` command, also in the root directory, should build the _parafeed_ package.

While the hand-written `makefiles` have been tested with `make` version 3 only, they do not depend on a specific version of `make`.  These `makefiles` are in the `code/nocmake_makefiles` directory. Using the command `cd code; make -f nocmake_makefiles/makefile.cllib.generic` should build the _parafeed_ project.

Successfully building and linking the test program `code/tstcpp` ensures that all libraries required by client codes are ready.  For now, copy all `code/*.{a,h}` and `code/calc/*.{a,h}` files to the required location for linking your application.   

A more detailed document describing the API is under prepration.  Till then, ask for help from the authors.
