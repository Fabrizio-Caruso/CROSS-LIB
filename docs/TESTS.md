## COMPILATION TESTS

We can test the compilation of the game for multiple targets by running some tests that build a very simple version of the game that may not be playable.
The purpose of these tests is to test the compilation.

### SIMPLE GCC-ONLY TESTS
If you have installed `GCC`, you can run `make simple_test` (short compilation time), which builds the game by only using `GCC` for the host system.
This test will produce a binary that can be run on the host console.

### TESTS ON SEVERAL COMPILERS AND TARGETS
You can run many tests that use several cross-compilers and dev-kits not including Z88DK by running `make no_z88dk_test`.

You can run many tests that use several cross-compilers and dev-kits including Z88DK by running `make test` (long compilation time).

### MORE Z88DK-ONLY TESTS
If you have installed `Z88DK` you can test lots of Z80 systems and targets by 
- `make sccz80_test` to test the compilation of different targets with `SCCZ80`;
- `make zsdcc_test` to test the compilation of different targets with `ZSDCC`
- `make z88dk_test` to cover both cross-compilers.

