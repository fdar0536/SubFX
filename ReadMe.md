# SubFX

A scripting tool for creating karaoke subtitle effects.

Now is under development.

## Dependency

### CoreLib

- C++14
- [Boost C++ Libraries](https://www.boost.org)
  - Regex with [ICU](http://site.icu-project.org)
  - Math (Headers only)
  - Locale (Windows only)
- [Pango](https://pango.gnome.org) (Non-Windows only)
- [pybind11](https://github.com/pybind/pybind11) (Optional)
- [Python3](https://www.python.org) or later (Optional)

### frontend

#### Common

- C++14
- [CoreLib] with Python
- [nlohmann's json](https://github.com/nlohmann/json)

#### CLI

- No extra dependency needed

## TODO

- [ ] Nicer name
- [ ] Nice frontend
- [ ] Document

## Known issue(s)

- GCC CANNOT compile this.

## Licence

[GPLv3](https://www.gnu.org/licenses/gpl-3.0.html)
