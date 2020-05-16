## COMPILATION TESTS

We can test the compilation of the game for multiple targets by running some tests that build a very simple version of the game that may not be playable.
The purpose of these tests is to test the compilation.

### GCC
If you have installed `GCC`, you can run `make simple_test` (short compilation time), which builds the game by only using `GCC` for the host system.
This test will produce a binary that can be run on the host console.


### Z88DK
If you have installed `Z88DK` you can test lots of Z80 systems and targets by 
- `make sccz80_test` to test the compilation of different targets with `SCCZ80`;
- `make zsdcc_test` to test the compilation of different targets with `ZSDCC`
- `make z88dk_test` to cover both cross-compilers.

Compilation for these tests will be very long.

### ALL TESTS
You can run all tests by running `make test` (long compilation time).