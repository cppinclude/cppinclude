# Changelog

## [v0.5.0](docs/versions/VERSION_0.5.0.md)

### Improved (v0.5.0)

* Perfomance with regex

### Added (v0.5.0)

* New argument *show_details*
* New argument *thousands_separator*
* New argument *verbose_ignore*

## [v0.4.0](docs/versions/VERSION_0.4.0.md)

### Added (v0.4.0)

* New argument *show_only_std_headers*

## [v0.3.1](docs/versions/VERSION_0.3.1.md)

### Fixed (v0.3.1)

* Fixed detect file name after comments /**/ in include

## [v0.3.0](docs/versions/VERSION_0.3.0.md)

### Added (v0.3.0)

* Support CMake ( JSON Compilation Database, compile_commands.json )
* Analyze unincluded headers
* Analyze headers that are included in different ways ( #include "..." and #include <...> )
* All arguments in configuration file

### Fixed (v0.3.0)

* Fixed bug in analyzing raw C++ strings
* Fixed bug in analyzing comments after #include

## [v0.2.1](docs/versions/VERSION_0.2.1.md)

### Fixed (v0.2.1)

* Fixed build for:
  * CentOS 7
  * GCC 7
  * Visual Studio 2017
  * Apple Clang 12

## [v0.2.0](docs/versions/VERSION_0.2.0.md)

### Added (v0.2.0)

* New parameter *analyze_without_extension*
* New argument *show_std_files*

### Fixed (v0.2.0)

* Fixed bug in multiline comments
* Fixed bug in multiline string
* Fixed bug in strig with quotation marks

