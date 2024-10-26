## BUILDING PROJECTS

We recommend to use `xl` (equivalent to `xl build`) to build projects.

### Using `xl`

I recommend that you use `xl build`:

`xl  [game_or_test_name] [optional system_name] [optional number of threads]`

A convenient shortcut is using just `xl` as follows:

`xl [project_name] [optional system_name] [optional number of threads]`

The `[optional system_name]` parameter is the name of the target. If no target is specified then the native target (the host terminal) is implied.

The `[optional number of threads]` is used to specify the number of threads to use for multiple targets that can be built in parallel.

Examples:
- `xl snake` -> It builds Cross Snake for the native console by using `gcc` and `ncurses`.
- `xl bomber atari` -> It builds Cross Bomber for the Atari 8-bit target (by using the appropriate cross-compiler, i.e., CC65)
- `xl snake vic20` -> It builds Cross Snake for the Commodore Vic 20.
- `xl games msx` -> It builds all game projects for the MSX target (by using the appropriate cross-compiler, i.e., the ones in Z88DK).
- `xl bomber cc65` -> It builds Cross Bomber for all targets that use CC65.
- `xl examples c64` -> It builds all examples for the Commodore 64 target

Remark: All binaries will be in the `build` directory (same depth level as `src`).

-------------------------------------------
## CREATING A NEW GAME PROJECT

In order to create a new game project we can use the `xl create` scripts that will create the necessary initial source code files, graphic assets and Makefile inside a folder in the `games` folder.

The script is used as follows:

`xl create [game project name] [initial code type]`

where `[initial code type]` can be 
- `helloworld` or empty for initial code that only displays 'hello world'
- `game` for an initial code for a standard game with a main loop and a level loop
- `demo` for an initial code that uses most APIs.

Examples:
- `xl create foo` -> It creates a new game project `foo` with a trivial code that initializes sound, input and graphics and just displays `hello world` on the screen.
- `xl create bar game` -> It creates a new game project `bar` with code that initializes sound, input and graphics and contains the main loops that may be used in a standard game.
- `xl create foobar demo` -> It creates a new game project `foobar` with code that initializes sound, input and graphics and contains code that shows how to use most APIs.

-------------------------------------------
## CLEANING TEMPORARY FILES AND BINARIES

If you  want to remove built binaries and temporary files that are produced during a build you can use 'xl clean'.

`xl clean`

--------------------------------------------
## DELETING PROJECTS

Non-built-in game projects can be deleted trhough the `xl delete` script in a very simple way:

`xl delete [game project name]`

Example: 

`xl delete foo` -> It removes the `foo` source code, assets and Makefile files
