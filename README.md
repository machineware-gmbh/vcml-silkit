# VCML SILKIT

This repository holds VCML models for SILKIT specific components.

[![Build Status](https://github.com/machineware-gmbh/vcml-silkit/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/machineware-gmbh/vcml-silkit/actions/workflows/cmake.yml)
[![ASAN Status](https://github.com/machineware-gmbh/vcml-silkit/actions/workflows/asan.yml/badge.svg?branch=main)](https://github.com/machineware-gmbh/vcml-silkit/actions/workflows/asan.yml)
[![Lint Status](https://github.com/machineware-gmbh/vcml-silkit/actions/workflows/lint.yml/badge.svg?branch=main)](https://github.com/machineware-gmbh/vcml-silkit/actions/workflows/lint.yml)
[![Style Status](https://github.com/machineware-gmbh/vcml-silkit/actions/workflows/style.yml/badge.svg?branch=main)](https://github.com/machineware-gmbh/vcml-silkit/actions/workflows/style.yml)
[![Nightly Status](https://github.com/machineware-gmbh/vcml-silkit/actions/workflows/nightly.yml/badge.svg?branch=main)](https://github.com/machineware-gmbh/vcml-silkit/actions/workflows/nightly.yml)
[![Coverage Status](https://github.com/machineware-gmbh/vcml-silkit/actions/workflows/coverage.yml/badge.svg?branch=main)](https://github.com/machineware-gmbh/vcml-silkit/actions/workflows/coverage.yml)

----
## Installation

Clone repository and update submodules:
```
git clone git@github.com:machineware-gmbh/vcml-silkit.git
git submodule update --init
```

Building `vcml-silkit` requires `cmake >= 3.11`. During configuration you
must state whether to build the unit tests:
* `-DVCML_SILKIT_TESTS=[ON|OFF]`: build unit tests (default `OFF`)
* `-DVCML_SILKIT_LINTER=<string>`: linter program to use (default `<empty>`)
* `-DVCML_SILKIT_COVERAGE=[ON|OFF]`: generate test coverage report (default `OFF`)

Note that when building this as a cmake submodule, unit tests will always be
disabled and the respective build option setting is ignored.
```
cmake -B RELEASE/BUILD           \
      -DCMAKE_BUILD_TYPE=RELEASE \
      -DCMAKE_INSTALL_PREFIX=..  \
      -DVCML_SILKIT_TESTS=ON .
cmake --build RELEASE/BUILD -- install
```
If everything went well, the following artifacts will have been installed:
* vcml-silkit headers in `RELEASE/include/`
* vcml-silkit library in `RELEASE/lib/libvcml-silkit.a`

----
## License

This project is licensed under the GNU General Public License, version 2 license - see the
[LICENSE](LICENSE) file for details.
