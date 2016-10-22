# des-cbc
DES implemented with CBC mode

Haven't done yet.

This program do DES algorithm with lots of bitwise operation.
It is my first time to use GNU Autotools to deploy project.
===
After, I add GUI of Qt lib, and try to compile it with Autotools successfully.

## Compile
Firstly, you should execute
```
autoreconf -i
```
to check environments and generate `./configure` script, then
```
./configure && make
```
to compile, type
```
./main
```
to run.
