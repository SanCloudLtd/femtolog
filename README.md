<!--
Copyright (c) 2022 SanCloud Ltd
SPDX-License-Identifier: CC-BY-4.0
-->

# femtolog

A simple logging library based on [log.c by rxi](https://github.com/rxi/log.c).

## Usage

This library may be built and installed using CMake. Alternatively, if
your application is also built using CMake, this repository can be
included as a subdirectory to give a more tightly coupled project.

To use this library from a C/C++ application, follow these simple steps:

  1) Include [femtolog.h](src/femtolog.h) from your C/C++ source files.

  2) Implement an output function which matches the `femtolog_output_fn`
     type.

  3) Call `femtolog_init()` early in your main function.

  4) Use the appropriate `log_*()` functions to emit log messages.

Additional configuration macros can be defined to customise the
behaviour of femtolog as documented in the header file.

Example usage is provided in
[femtolog-example.c](src/femtolog-example.c) and HTML documentation can
be built using doxygen.

## Maintainers

* Paul Barker
  [:envelope:](mailto:paul.barker@sancloud.com)

## Contributing

Issues and Pull Requests for this project may be opened on our primary
GitHub repository at https://github.com/SanCloudLtd/femtolog. When
submitting a pull request, all commits should include a `Signed-off-by`
line to indicate that your contribution is made in accordance with the
[Developer Certificate of Origin](https://developercertificate.org/).

## License

Copyright (c) 2021-2022 SanCloud Ltd.

* Code files are distributed under the
  [MIT License](https://tldrlegal.com/license/mit-license).

* Documentation files are distributed under the
  [CC BY 4.0 License](https://tldrlegal.com/license/creative-commons-attribution-4.0-international-(cc-by-4)).

* Trivial data files are distributed under the
  [CC0 1.0 License](https://tldrlegal.com/license/creative-commons-cc0-1.0-universal).
