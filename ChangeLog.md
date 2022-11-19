<!--
Copyright (c) 2022 SanCloudLtd
SPDX-License-Identifier: CC-BY-4.0
-->

# ChangeLog for femtolog

## 0.2.1

Minor patch release.

Changes from v0.2.0:

* Only use printf function attribute when building with GCC or clang
  (since other compilers such as MSVC may not support this).

* Adopt a consistent code style, enforced via clang-format.

* Add GitLab CI configuration.

* Add contribution notes to the readme file.

* Explicitly initialise the internal log state object.

* Confirm output function is non-NULL before calling it.

* Improve release scripting and add release directory to the git ignore
  list.

## 0.2.0

Initial public release. Refactored, improved and documented.

## 0.1.0

Internal pre-release.
