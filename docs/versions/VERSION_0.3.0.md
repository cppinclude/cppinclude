# VERSION 0.3.0

## Added

### New parameter: compile_commands

Path to JSON Compilation Database. You can set in configuration file:

```json
{
    "compile_commands" : "build/compile_commands"
}
```

or in arguments:

`cppinclude --compile_commands=build/compile_commands.json`

And *cppinclude* will analyze source files and recursively headers files

### New report: unincluded

Show unincluded headers. You can set in configuration file:

```json
{
    "report" : [ "unincluded" ]
}
```

or in arguments:

`cppinclude --report=unincluded`

Example from [Clementine](docs/examples/clementine)

```
cppinclude --report=unincluded
...
Unincluded headers:
1 : "src/mac/SBSystemPreferences.h"
2 : "src/internet/lastfm/lastfmurlhandler.h"
3 : "tests/mock_sqldriver.h"
...
```

### New report different_type

Show headers that included in #include <...> and #include "..." .
You can set in configuration file:

```json
{
    "report" : [ "different_type" ]
}
```

or in arguments:

`cppinclude --report=different_type`

Example from [simple_example_for_different_type_report](docs/examples/simple_example_for_different_type_report)

```
cppinclude --report=different_type
...
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
...
```

### All arguments in configuration file

Now you can set in configuration file *report*, *report_limit*
and *report_details_limit*

Example from [lua](docs/examples/lua)

```json
{
    "report" : [
        "most_impact", "unresolved"
    ],

    "report_limit" : 30,
    "report_details_limit" : 30
}
```

## Fixed

### Fixed bug in analyzing raw string

Example:

```c++
std::string str = R"(
        #include <header.hpp>
    )";

```

**Before:** File header.hpp was detected

**After:** It's detected as string

### Fixed bug in analyzing comments after #include

Example:

```c++
#include "lib1" //  <lib2>

```

**Before:** lib2 was detected

**After:** lib1 is detected

