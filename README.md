# CPPINCLUDE

Tool that helps to analyze includes in C++.
One of the problem in C++ is that if header file was changed all files that
include the file will be recompiled and sometime it takes a lot of time.

## Table of Contents

* [Examples](#examples)
* [Settings](#setting)
  * [All arguments](#all-arguments)
  * [configuration_file](#configuration_file)
  * [project_dir](#project_dir)
  * [file_extensions](#file_extensions)
  * [include_dirs](#include_dirs)
  * [ignore_dirs](#ignore_dirs)
  * [ignore_system_includes](#ignore_system_includes)
  * [ignore_files](#ignore_files)
  * [report](#report)
  * [report_limit](#report_limit)
  * [report_details_limit](#report_details_limit)
* [Build](#build)
* [Presentations](#presentation)
* [Tips for optimization includes](#tips-for-optimization-includes)
* [Third-party libraries](#third-party-libraries)
* [Support](#support)

## Examples

Example from [docs/examples/simple_example/](docs/examples/simple_example/)

* file *base_char_factory.hpp*

<!-- markdownlint-disable -->

```c++
#pragma once
#include "base_char.hpp"
#include <memory>

class BaseCharFactory
{
public:
    virtual ~BaseCharFactory() = default;
    virtual std::unique_ptr< BaseChar > createObject() = 0;
};
```

<!-- markdownlint-enable -->

* file *base_char.hpp*

```c++
#pragma once
#include "char_kind.hpp"

class BaseChar
{
public:
    virtual ~BaseChar() = default;
    virtual CharKind getKind() const noexcept = 0;
};

```

If file *char_kind.hpp* is changed all files that include *base_char_factory.hpp*
and *base_char.hpp* will be recompile and it will take time. This tool helps to find
file in top of include hierarchy:

```
cppinclude
...
Most impact files:
1 : "char_kind.hpp" impact to 11 file(s)
Included by:
   1 : "base_char.hpp" line 3, impact to 10 file(s)
2 : "base_char.hpp" impact to 10 file(s)
Included by:
    1 : "base_char_factory.hpp" line 3, impact to 5 file(s)
    2 : "char_a.hpp" line 3, impact to 2 file(s)
    3 : "char_b.hpp" line 3, impact to 2 file(s)
3 : "base_char_factory.hpp" impact to 5 file(s)
Included by:
    1 : "char_a_factory.hpp" line 3, impact to 2 file(s)
    2 : "char_b_factory.hpp" line 3, impact to 2 file(s)
...
```

See more examples in [docs/examples/](docs/examples/)

[Back to top](#cppinclude)

## Settings

### All arguments

Name|Short description
------------ | -------------
--[configuration_file](#configuration_file)=file|Path to configuration file (default: .cppinclude.json)
--[project_dir](#project_dir)=dir|Project directory
--[file_extensions](#file_extensions)=arg1,arg2,...|Extensions C++ files (default: \*.cpp, \*.hpp,\*.c,\*.h,\*.cxx,\*.hxx)
--[include_dirs](#include_dirs)=dir1,dir2,...|Include directories
--[ignore_dirs](#ignore_dirs)=dir1,dir2,...|Directories that will be ignored
--[ignore_system_includes](#ignore_system_includes)=true|Ignore headers in \<\>
--[ignore_files](#ignore_files)=regexp1,regexp2,...|Files will be ignored by regexp
--[report](#report)=name1,name2,...|List reports (default: unresolved,most_impact)
--[report_limit](#report_limit)=42|Maximum elements in report, 0 - unlimited (default: 10)
--[report_details_limit](#report_details_limit)=42|Maximum details in report, 0 - unlimited (default: 10)
--help|Show usage
--verbose|Verbose mode
--version|Show application version

[Back to top](#cppinclude)

### configuration_file

The tool read setting from *.cppinclude.json* in work directory or
you can set file in argument *configuration_file*.
For example:

`cppinclude --configuration_file=project.json`

[Back to top](#cppinclude)

### project_dir

Path to folder with sources. Often source files are located in *src* or
*sources* folder, not in root folder of project.
You can set in configuration file:

```json
{
    "project_dir" : "src"
}
```

or in arguments:

`cppinclude --project_dir=src`

[Back to top](#cppinclude)

### file_extensions

If you use file extensions for C++ that aren’t in default values you can set in
configuration file:

```json
{
    "file_extensions" : ["*.cc", "*.hh"]
}
```

or in arguments:

`cppinclude --file_extensions=*.cc,*hh`

[Back to top](#cppinclude)

### include_dirs

Add folders where search included files. Default value is project folder.
You can set in configuration file:

```json
{
    "include_dirs" : [ "lib1", "lib2"]
}
```

or in arguments:

`cppinclude --include_dirs=lib1,lib2`

[Back to top](#cppinclude)

### ignore_dirs

Folders that will be ignored during analyzing project’s files.
It can be third-party libraries that are located in project directory
but don't need to analyze. You can set in configuration file:

```json
{
    "ignore_dirs" : ["./3rd-part", "gtest"]
}
```

or in arguments:

`cppinclude --ignore_dirs=./3rd-part,gtest`

[Back to top](#cppinclude)

### ignore_system_includes

Ignore includes with *\<\>*, example *#include \<iostream\>* will be ignored.
You can set in configuration file:

```json
{
    "ignore_system_includes" : true
}
```

or in arguments:

`cppinclude --ignore_system_includes=true`

[Back to top](#cppinclude)

### ignore_files

Ignore files by regexp. The tool will ignore files in project’s directory
and files in includes. For example, ignore all boost files
or generated files (*.gen). You can set in configuration file

```json
{
    "ignore_files" : [ "boost/.*", ".*\\.def"]
}
```

or in arguments:

`cppinclude --ignore_files=boost/.*,.*\\.def`

[Back to top](#cppinclude)

### report

Name of report. Possible values:

* *unresolved* -- show included files that are not found in project folder;
* *most_impact* -- show files that most impact to other files.

This report show how many files will be recompiled if the file is changes

```
cppinclude --report=unresolved
cppinclude --report=most_impact
cppinclude --report=unresolved,most_impact
```

[Back to top](#cppinclude)

### report_limit

Maximum number of files in report. For example,
only 5 unresolved files will be in report:

`cppinclude --report=unresolved --report_limit=5`

[Back to top](#cppinclude)

### report_details_limit

Maximum number of detail in report.
For example, only 3 files will be in report that include unresolved file

`cppinclude --report=unresolved --report_details_limit=3`

[Back to top](#cppinclude)

## Build

Requirements:

* C++17
* Cmake
* GCC/Clang/Visual Studio

Build script
on Windows:

```bat
.\build.bat
```

on Unix:

```sh
./build.sh
```

[Back to top](#cppinclude)

## Presentations

* 2020, internal talk - [2020_internal_talk.pdf](docs/slides/2020_internal_talk.pdf)

[Back to top](#cppinclude)

## Tips for optimization includes

* use forward declaration
* split big header
* [Pointer to implementation](https://en.cppreference.com/w/cpp/language/pimpl)

[Back to top](#cppinclude)

## Third-party libraries

* [cpp-std-fwd](https://github.com/Philip-Trettner/cpp-std-fwd)
* [cxxopts](https://github.com/jarro2783/cxxopts)
* [{fmt}](https://github.com/fmtlib/fmt)
* [json for modern C++](https://github.com/nlohmann/json)
* [Boost.Test](https://www.boost.org/)

[Back to top](#cppinclude)

## Support

If you need help with your project or need some feature please
write email to cppinclude@yandex.com

[Back to top](#cppinclude)

