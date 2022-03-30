# c-utils

Datastructures and utilities for C programming.


A standalone library conforming to C99-standard, **targeted for x64 platforms**.
This library does not use the C standard library, so it assumes a 64-bit target platform.
The code should be easy to port other targets, but all development in this repository
always assumes a 64-bit target platform.


## Including into your own projects

Get the source files easily with **wget**:

    wget https://raw.githubusercontent.com/varajala/c-utils/release/latest/c-utils.h
    wget https://raw.githubusercontent.com/varajala/c-utils/release/latest/c-utils.c

Build the library as a part of your own project, or use the provided makefile
to build a shared library.

Getting the makefile:

    wget https://raw.githubusercontent.com/varajala/c-utils/release/latest/makefile

Build:

    make release


## License

This project is licensed under the [MIT License](https://choosealicense.com/licenses/mit/).
See the [LICENSE](https://github.com/varajala/c-utils/blob/main/LICENSE).
