# SubFX

[![Build Status](https://travis-ci.com/fdar0536/SubFX.svg?branch=master)](https://travis-ci.com/fdar0536/SubFX)
[![Build Status](https://ci.appveyor.com/api/projects/status/github/fdar0536/SubFX?svg=true)](https://ci.appveyor.com/api/projects/status/github/fdar0536/SubFX?svg=true)

A scripting tool for creating karaoke subtitle effects.

Now is under development.

## Dependencies

### YutilsCpp

- C++14
- [Boost C++ Libraries](https://www.boost.org)
  - Regex with [ICU](http://site.icu-project.org)
  - Lexical Cast
  - Locale (Windows only)
- [Pango](https://pango.gnome.org) (Non-Windows only)
- [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page)
- [BLAS api](https://en.wikipedia.org/wiki/Basic_Linear_Algebra_Subprograms) (Optional)
  - for example: [LAPACK](https://www.netlib.org/lapack), [OpenBLAS](https://github.com/xianyi/OpenBLAS), [Intel MKL](https://software.intel.com/en-us/mkl)
- [pybind11](https://github.com/pybind/pybind11) (Optional)
- [Python3](https://www.python.org) or later (Optional)

### Wrapper

#### Common

- C++14
- YutilsCpp with Python
- [nlohmann's json](https://github.com/nlohmann/json)

#### CLI

- [cxxopts](https://github.com/jarro2783/cxxopts)

#### GUI

- [Qt](https://www.qt.io) 5.14 or later (5.12lts is not tested)

## TODO

- [ ] Nicer name
- [ ] Nice wrapper
- [ ] Document

## Licence

Except for the following components, all others are distributed under [LGPLv2.1](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html):
- [AssPy](src/YutilsCpp/python/asspy.py) and [its example(s)](src/YutilsCpp/python/examples): [GPLv3](https://www.gnu.org/licenses/gpl-3.0.html)
- All others examples and testing cases: [MIT](https://opensource.org/licenses/MIT)
