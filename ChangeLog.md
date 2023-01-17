<!--
Copyright (c) 2022 SanCloudLtd
SPDX-License-Identifier: CC-BY-4.0
-->

# ChangeLog for femtolog

## 0.2.2 (unreleased)

Minor patch release

Changes from v0.2.1:

* Mark `log_fatal_exit()` as `noreturn`.

* Make installation optional and off by default. Set the cmake variable
  `FEMTOLOG_INSTALL=ON` to enable installation of the femtolog static library,
  header file and example binary.

* Update coding style to require braces around single line blocks.

For more details, see the
[full commit log](https://github.com/SanCloudLtd/femtolog/compare/v0.2.1...main).

## 0.2.1

Minor patch release.

Changes from v0.2.0:

* Only use printf function attribute when building with GCC or clang
  (since other compilers such as MSVC may not support this).

* Explicitly initialise the internal log state object.

* Confirm output function is non-NULL before calling it.

* Add contribution notes to the readme file.

* Adopt a consistent code style, enforced via clang-format for C code
  and black for Python scripts.

* Add GitLab CI configuration.

* Improve release scripting and add release directory to the git ignore
  list.

For more details, see the
[full commit log](https://github.com/SanCloudLtd/femtolog/compare/v0.2.0...v0.2.1).

## 0.2.0

Initial public release. Refactored, improved and documented.

## 0.1.0

Internal pre-release.
