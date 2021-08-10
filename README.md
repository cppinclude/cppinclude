# CPPINCLUDE

Tool for analyzing includes in C++.
One of the problems in C++ is that if a header file was changed all files that
include that file will be recompiled and sometime it takes a lot of time.

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
  * [show_std_files](#show_std_files)
  * [show_only_std_headers](#show_only_std_headers)
  * [show_details](#show_details)
  * [thousands_separator](#thousands_separator)
  * [verbose_ignore](#verbose_ignore)
* [Build](#build)
* [Docker image](#docker-image)
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

If file *char_kind.hpp* is changed all files that
include *base_char_factory.hpp* and *base_char.hpp* will be recompiled and
it will take time. This tool helps to find the file at the top of
the include hierarchy:

```
cppinclude
...
Most impact files:
1 : "char_kind.hpp" impact on 11 files
Included by:
   1 : "base_char.hpp" line 3, impact on 10 files
2 : "base_char.hpp" impact on 10 files
Included by:
    1 : "base_char_factory.hpp" line 3, impact on 5 files
    2 : "char_a.hpp" line 3, impact on 2 files
    3 : "char_b.hpp" line 3, impact on 2 files
3 : "base_char_factory.hpp" impact on 5 files
Included by:
    1 : "char_a_factory.hpp" line 3, impact on 2 files
    2 : "char_b_factory.hpp" line 3, impact on 2 files
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
--[show_only_std_headers](#show_only_std_headers)|Show only standard library headers in output (default: false)
--[show_details](#show_details)|Show details in output (default: true)
--[thousands_separator](#thousands_separator)=separator|Set thousands separator, for example ',' (default: ' ')
--help|Show usage
--verbose|Verbose mode
--version|Show application version
--[verbose_ignore](#verbose_ignore)|Show ignored files

[Back to top](#cppinclude)

### configuration_file

The tool reads settings from *.cppinclude.json* located in the work directory or
you can pass a configuration file in the argument *configuration_file*.
For example:

`cppinclude --configuration_file=project.json`

[Back to top](#cppinclude)

### compile_commands

Path to generated *compile_commands.json* file by CMake with argument
*-DCMAKE_EXPORT_COMPILE_COMMANDS=ON*, for example:

`cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON`

You can set the path for the compile_command.json file in the configuration file:

```json
{
    "compile_commands" : "build/compile_commands.json"
}
```

or, pass as an argument:

`cppinclude --compile_commands=build/compile_commands.json`

[Back to top](#cppinclude)

### project_dir

PPath to a folder that contains sources. Often source files are located
in *src* or *sources* folder, not in the root folder of project.
You can set it in configuration file with:

```json
{
    "project_dir" : "src"
}
```

or, pass as an argument:

`cppinclude --project_dir=src`

[Back to top](#cppinclude)

### file_extensions

If you use other file extensions than default values *file_extensions*
for C++ sources you can specify them in the configuration file with:

```json
{
    "file_extensions" : ["*.cc", "*.hh"]
}
```

or, pass as an argument:

`cppinclude --file_extensions=*.cc,*hh`

[Back to top](#cppinclude)

### analyze_without_extension

Analyze files without extension in the project directory, default: false.
You can set this option in the configuration file with:

```json
{
    "analyze_without_extension" : true
}
```

or, pass as an argument:

`cppinclude --analyze_without_extension=true`

[Back to top](#cppinclude)

### include_dirs

Add paths to the header search directories. Default value is project folder.
You can set it in the configuration file with:

```json
{
    "include_dirs" : [ "lib1", "lib2"]
}
```

or, pass as an argument:

`cppinclude --include_dirs=lib1,lib2`

[Back to top](#cppinclude)

### ignore_dirs

Folders to be ignored during project analysis.
For example: third-party libraries that are located under the project directory
but should not be analyzed.
You can set it in the configuration file with:

```json
{
    "ignore_dirs" : ["./3rd-part", "gtest"]
}
```

or, pass as an argument:

`cppinclude --ignore_dirs=./3rd-part,gtest`

[Back to top](#cppinclude)

### ignore_system_includes

Ignore includes with *\<\>*, example *#include \<iostream\>* will be ignored.
You can set it in the configuration file with:

```json
{
    "ignore_system_includes" : true
}
```

or, pass as an argument:

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

or, pass as an argument:

`cppinclude --ignore_files=boost/.*,.*\\.def`

[Back to top](#cppinclude)

### report

Name of report. Possible values:

* *unresolved* -- show included files that are not found within the project folder;
* *most_impact* -- show files that most impact on other files;
* *unincluded* -- show unincluded headers;
* *different_type* -- show headers that are included in #include <...> and #include "..." .

As arguments:

```
cppinclude --report=unresolved
cppinclude --report=most_impact
cppinclude --report=unresolved,most_impact
```

As a configuration file setting:

```json
{
    "report" : [ "unresolved", "most_impact"]
}
```

[Back to top](#cppinclude)

#### unresolved

Show headers that are included but not found in the given search paths.
One possible reason is missing include files, for example:
iven a *main.cpp* file and a *include* folder that stores
the *header.hpp* header:

```
tree
.
├── include
│   └── header.hpp
└── main.cpp

```

When *cppinclude* is run, the result will be:

```
cppinclude --report=unresolved
Start initialization project ...
Start analyze sources ...
Start report results ...
Unresolved files:
1. "header.hpp" isn't resolved in:
    1. "main.cpp" line: 1

```

Adding the *include* folder to *include_dirs* will remove the
unresolved files entry:

```
cppinclude --report=unresolved --include_dirs=include
```

[Back to top](#cppinclude)

#### most_impact

Show how many files will be recompiled when a given header is changed.
Example from [docs/examples/simple_example/](docs/examples/simple_example/)

```
cppinclude --report=most_impact
...
Most impact files:
1 : "char_kind.hpp" impact on 11 files
Included by:
   1 : "base_char.hpp" line 3, impact on 10 files
2 : "base_char.hpp" impact on 10 files
Included by:
    1 : "base_char_factory.hpp" line 3, impact on 5 files
    2 : "char_a.hpp" line 3, impact on 2 files
    3 : "char_b.hpp" line 3, impact on 2 files
3 : "base_char_factory.hpp" impact on 5 files
Included by:
    1 : "char_a_factory.hpp" line 3, impact on 2 files
    2 : "char_b_factory.hpp" line 3, impact on 2 files
...
```

The above output means that a change in char_kind.hpp will force 11
files to recompile.

[Back to top](#cppinclude)

#### unincluded

Show headers that are found in the search directories but that were not
included in the source code.
It often happens after refactoring when headers when include directives
were removed from code and thefiles remained in place.
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

* Headers with same names:

If headers have the same name but are located in different paths only one
occurrence will be counted.
only first header and other will be unincluded.
For example: *lib1/header.hpp*, *lib2/header.hpp* and *main.cpp* :

```c++
#include "header.hpp"
...
```

The result will be:

```
cppinclude --include_dirs=lib1,lib2 --report=unincluded

Start initialization project ...
Start analyze sources ...
Start report results ...
Unincluded headers:
1 : "lib2/header.hpp"

```

* Empty result for CMake project:

The current implementation ignores CMake project files.
Only source files are analyzed currently that are either specified
in compile_commands.json or that are found on the filesystem.

* Header files are files that have extension started with *h* letter

***All limitations will be fixed in future releases***

[Back to top](#cppinclude)

#### different_type

Show headers that are included in different ways.
It helps to follow code styles in project,
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

### show_only_std_headers

Show only standard library headers in output.

`cppinclude --show_only_std_headers=true`

Also you can set in configuration file:

```json
{
    "show_only_std_headers" : true
}
```

### show_details

Show or hide details

`cppinclude --show_details=false`

Also you can set in configuration file:

```json
{
    "show_details" : false
}
```

Example [lua](docs/examples/lua) :

```
cppinclude --show_details=false

...
Most impact files:
1 : "luaconf.h" impact on 62 files
2 : "lua.h" impact on 61 files
3 : "llimits.h" impact on 40 files
...

```

Don't show where files are included

[Back to top](#cppinclude)

### thousands_separator

You can change default thousands separator (space) to your separator

For example:

`cppinclude --thousands_separator=,`

Also you can set in configuration file:

```json
{
    "thousands_separator" : ","
}
```

Result:

```
...
Most impact files:
1 : "supper_header.h" impact on 123,456 files
2 : "config.h" impact on 12,345 files
3 : "limits.h" impact on 1,234 files
...

```

**Note:** you can use only one character

Don't show where files are included

[Back to top](#cppinclude)

### verbose_ignore

Show ignored files and folders. For example:

```
cppinclude --verbose_ignore

Path "sys/types.h" was ignored by "sys/.*.h"
Path "mach/mach_init.h" was ignored by "mach/.*.h"
Folder ".../3rdparty" was ignored
...

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

## Docker image

Pull image:

```sh
docker pull cppinclude/cppinclude
```

Or build docker image from source:

```sh
docker build -t cppinclude/cppinclude .
```

Test image:

```sh
docker run -it cppinclude/cppinclude bash
cppinclude --version
```

Analyze your sources in docker image:

```sh
docker run -v /tmp/project_src:/src/project_src -it cppinclude/cppinclude
cd /src/project_src
cppinclude
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

* [stdfwd](https://github.com/olegpublicprofile/stdfwd)
* [cxxopts](https://github.com/jarro2783/cxxopts)
* [{fmt}](https://github.com/fmtlib/fmt)
* [json for modern C++](https://github.com/nlohmann/json)
* [Boost.Test](https://www.boost.org/)
* [RE2](https://github.com/google/re2)

[Back to top](#cppinclude)

## Support

If you need help with your project or need some feature please
write email to cppinclude@yandex.com

[Back to top](#cppinclude)

