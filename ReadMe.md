# SubFX

[![Build Status](https://travis-ci.com/fdar0536/SubFX.svg?branch=master)](https://travis-ci.com/fdar0536/SubFX)

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
- [pybind11](https://github.com/pybind/pybind11) (Optional)
- [Python3](https://www.python.org) or later (Optional)

### frontend

#### Common

- C++14
- YutilsCpp with Python
- [nlohmann's json](https://github.com/nlohmann/json)

#### CLI

- No extra dependency needed

#### GUI

- [Qt](https://www.qt.io) 5.14 or later (5.12lts is not tested)

## TODO

- [ ] Nicer name
- [ ] Nice frontend
- [ ] Document

## Licence

[GPLv3](https://www.gnu.org/licenses/gpl-3.0.html)
