# CPPINCLUDE

Tool for analyzing includes in C++.
One of the problem in C++ is that if header file was changed all files that
include the file will be recompiled and sometime it takes a lot of time.

## Table of Contents

* [Examples](#examples)
* [Settings](#setting)
  * [All arguments](#all-arguments)
  * [report](#report)
    * [unresolved](#unresolved)
    * [most_impact](#most_impact)
    * [unincluded](#unincluded)
    * [different_type](#different_type)
  * [configuration_file](#configuration_file)
  * [compile_commands](#compile_commands)
  * [project_dir](#project_dir)
  * [file_extensions](#file_extensions)
  * [analyze_without_extension](#analyze_without_extension)
  * [include_dirs](#include_dirs)
  * [ignore_dirs](#ignore_dirs)
  * [ignore_system_includes](#ignore_system_includes)
  * [ignore_files](#ignore_files)
  * [report_limit](#report_limit)
  * [report_details_limit](#report_details_limit)
* [Build](#build)
* [Presentations](#presentations)
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
1 : "char_kind.hpp" impact on 11 file(s)
Included by:
   1 : "base_char.hpp" line 3, impact on 10 file(s)
2 : "base_char.hpp" impact on 10 file(s)
Included by:
    1 : "base_char_factory.hpp" line 3, impact on 5 file(s)
    2 : "char_a.hpp" line 3, impact on 2 file(s)
    3 : "char_b.hpp" line 3, impact on 2 file(s)
3 : "base_char_factory.hpp" impact on 5 file(s)
Included by:
    1 : "char_a_factory.hpp" line 3, impact on 2 file(s)
    2 : "char_b_factory.hpp" line 3, impact on 2 file(s)
...
```

See more examples in [docs/examples/](docs/examples/)

[Back to top](#cppinclude)

## Settings

### All arguments

Name|Short description
------------ | -------------
--[report](#report)=name1,name2,...|List reports. Name of reports: [unresolved](#unresolved), [most_impact](#most_impact), [unincluded](#unincluded), [different_type](#different_type) (default: [unresolved](#unresolved),[most_impact](#most_impact),[unincluded](#unincluded))
--[configuration_file](#configuration_file)=file|Path to configuration file (default: .cppinclude.json)
--[compile_commands](#compile_commands)=file|Path to JSON Compilation Database (default: compile_commands.json)
--[project_dir](#project_dir)=dir|Project directory
--[file_extensions](#file_extensions)=arg1,arg2,...|Extensions C++ files (default: \*.cpp, \*.hpp,\*.c,\*.h,\*.cxx,\*.hxx)
--[analyze_without_extension](#analyze_without_extension)=true|Analyze files without extension (default: false)
--[include_dirs](#include_dirs)=dir1,dir2,...|Include directories
--[ignore_dirs](#ignore_dirs)=dir1,dir2,...|Directories that will be ignored
--[ignore_system_includes](#ignore_system_includes)=true|Ignore headers in \<\> (default: false)
--[ignore_files](#ignore_files)=regexp1,regexp2,...|Files will be ignored by regexp
--[report_limit](#report_limit)=42|Maximum elements in report, 0 - unlimited (default: 10)
--[report_details_limit](#report_details_limit)=42|Maximum details in report, 0 - unlimited (default: 10)
--[show_std_files](#show_std_files)|Show standard library headers in output (default: false)
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

### compile_commands

Path to generated *compile_commands.json* file by CMake with argument
*-DCMAKE_EXPORT_COMPILE_COMMANDS=ON*, for example:

`cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON`

You can set path in configuration file:

```json
{
    "compile_commands" : "build/compile_commands.json"
}
```

or in arguments:

`cppinclude --compile_commands=build/compile_commands.json`

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

If you use file extensions for C++ that aren’t in default values. You can set in
configuration file:

```json
{
    "file_extensions" : ["*.cc", "*.hh"]
}
```

or in arguments:

`cppinclude --file_extensions=*.cc,*hh`

[Back to top](#cppinclude)

### analyze_without_extension

Analyze files in project directory without extension, default: false.
You can set in configuration file:

```json
{
    "analyze_without_extension" : true
}
```

or in arguments:

`cppinclude --analyze_without_extension=true`

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
* *most_impact* -- show files that most impact on other files;
* *unincluded* -- show unincluded headers;
* *different_type* -- show headers that are included in #include <...> and #include "..." .

```
cppinclude --report=unresolved
cppinclude --report=most_impact
cppinclude --report=unresolved,most_impact
```

Also you can set in configuration file:

```json
{
    "report" : [ "unresolved", "most_impact"]
}
```

[Back to top](#cppinclude)

#### unresolved

Show files that are found in includes but didn't found in file system.
One of the resean is missing includes files, for example:
There is file *main.cpp*, folder *include* that store file *header.hpp* :

```
tree
.
├── include
│   └── header.hpp
└── main.cpp

```

If run *cppinclude* result will be:

```
cppinclude --report=unresolved
Start initialization project ...
Start analyze sources ...
Start report results ...
Unresolved files:
1. "header.hpp" isn't resolved in:
    1. "main.cpp" line: 1

```

But if add folder *include* to *include_dirs* resultat will be empty:

```
cppinclude --report=unresolved --include_dirs=include
```

[Back to top](#cppinclude)

#### most_impact

Show how many files will be recompiled with header will be changed.
Example from [docs/examples/simple_example/](docs/examples/simple_example/)

```
cppinclude --report=most_impact
...
Most impact files:
1 : "char_kind.hpp" impact on 11 file(s)
Included by:
   1 : "base_char.hpp" line 3, impact on 10 file(s)
2 : "base_char.hpp" impact on 10 file(s)
Included by:
    1 : "base_char_factory.hpp" line 3, impact on 5 file(s)
    2 : "char_a.hpp" line 3, impact on 2 file(s)
    3 : "char_b.hpp" line 3, impact on 2 file(s)
3 : "base_char_factory.hpp" impact on 5 file(s)
Included by:
    1 : "char_a_factory.hpp" line 3, impact on 2 file(s)
    2 : "char_b_factory.hpp" line 3, impact on 2 file(s)
...
```

It means if file char_kind.hpp will be changed 11 files are recompiled.

[Back to top](#cppinclude)

#### unincluded

Show files that are found in file system but didn't found in includes.
It often happens after refactoring when file that include header was deleted.
Example from [docs/examples/simple_example_with_unincluded_headers/](docs/examples/simple_example_with_unincluded_headers/)

```
cppinclude --report=unincluded
Start initialization project ...
Start analyze sources ...
Start report results ...
Unincluded headers:
1 : "config.hpp"
2 : "settings.hpp"

```

**Limitations:**

* Header with same names: 

If headers have same name but are located in different folders will be found 
only first header and other will be unincluded. 
For example: *lib1/header.hpp*, *lib2/header.hpp* and
*main.cpp* : 

```c++
#include "header.hpp"
...
```
Result will be:

```
cppinclude --include_dirs=lib1,lib2 --report=unincluded

Start initialization project ...
Start analyze sources ...
Start report results ...
Unincluded headers:
1 : "lib2/header.hpp"

```

* Empty result for CMake project: 

If analyze CMake project ( generated file compile_commands.json ) 
result will be empty. Because in current implementation *cppinclude* analyze 
source files on file system or files from compile_commands.json

* Header files are files that have extension started with *h* letter

***All limitations will be fixed in future releases ***


[Back to top](#cppinclude)

#### different_type

Show headers that are included in different ways.
It helps to follow code style in project,
for example include third party libraries in <...> and project header in "...".
Example from [docs/examples/simple_example_for_different_type_report/](docs/examples/simple_example_for_different_type_report/)

```
cppinclude --report=different_type
Start initialization project ...
Start analyze sources ...
Start report results ...
Files that are included by different ways:
1. base_char.hpp
With double quotation marks ( #include "..." ) in files:
    1. base_char_factory.hpp line 3
    2. char_b.hpp line 3
With angle brackets ( #include <...> ) in files:
    1. char_a.hpp line 3
2. base_char_factory.hpp
With double quotation marks ( #include "..." ) in files:
    1. char_b_factory.hpp line 3
With angle brackets ( #include <...> ) in files:
    1. char_a_factory.hpp line 3
```

[Back to top](#cppinclude)

### report_limit

Maximum number of files in report. For example,
only 5 unresolved files will be in report:

`cppinclude --report=unresolved --report_limit=5`

Also you can set in configuration file:

```json
{
    "report_limit" : 5
}
```

[Back to top](#cppinclude)

### report_details_limit

Maximum number of detail in report.
For example, only 3 files will be in report that include unresolved file

`cppinclude --report=unresolved --report_details_limit=3`

Also you can set in configuration file:

```json
{
    "report_details_limit" : 3
}
```

[Back to top](#cppinclude)

### show_std_files

Enable showing standard library headers in output.

`cppinclude --show_std_files=true`

Also you can set in configuration file:

```json
{
    "show_std_files" : true
}
```

[Back to top](#cppinclude)

## Build

Requirements:

* C++17
* CMake
* Compilers:
  * GCC ( tested on 7.5 and 10.1 versions )
  * Visual Studio ( tested on 2017 and 2019 community edition versions  )
  * Clang ( tested on 10.0 version )
  * Apple Clang ( tested on 12.0 version )

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

* 2020, Lightning Talk on CppCon 2020, cppinclude - Tool for analyzing includes in C++
  * Video : [https://www.youtube.com/watch?v=DXil_ahLTyg](https://www.youtube.com/watch?v=DXil_ahLTyg)
  * Slides: [2020_cppcon2020_small_talk.pdf](docs/slides/2020_cppcon2020_small_talk.pdf)
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

