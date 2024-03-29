# SubFX

A scripting tool for creating karaoke subtitle effects.

Now is under development.

## Dependencies

### YutilsCpp

- C++14
- [fDSA](https://github.com/fdar0536/fdsa)
- [PCRE2](https://www.pcre.org/)
- [Pango](https://pango.gnome.org) (Non-Windows only)
- [pybind11](https://github.com/pybind/pybind11) (Optional)
- [Python3](https://www.python.org) or later (Optional)

### Wrapper

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

Except for the following components, all others are distributed under [LGPLv2.1](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html):
- [AssPy](src/YutilsCpp/python/asspy.py) and [its example(s)](src/YutilsCpp/python/examples): [GPLv3](https://www.gnu.org/licenses/gpl-3.0.html)
- All others examples and testing cases: [MIT](https://opensource.org/licenses/MIT)
