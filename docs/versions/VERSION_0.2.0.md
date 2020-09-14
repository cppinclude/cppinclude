# VERSION 0.2.0

## Added

### New parameter analyze_without_extension

Analyze files in project directory without extension, default: false.
You can set in configuration file:

```json
{
    "analyze_without_extension" : true
}
```

or in arguments:

`cppinclude --analyze_without_extension=true`

### New argument show_std_files

Show standard library headers in output. Example [lua](docs/examples/lua) :

**Before:**

```
cppinclude

Start initialization project ...
Start analyze sources ...
Start report results ...
Most impact files:
1 : "luaconf.h" impact on 62 file(s)
Included by:
    1 : "lua.h" line 16, impact on 61 file(s)
    2 : "onelua.c" line 49
2 : "lua.h" impact on 61 file(s)
Included by:
    1 : "llimits.h" line 15, impact on 40 file(s)
    2 : "lobject.h" line 16, impact on 34 file(s)
    3 : "lstate.h" line 10, impact on 26 file(s)
    4 : "lauxlib.h" line 15, impact on 18 file(s)
    5 : "lmem.h" line 14, impact on 15 file(s)
    6 : "lualib.h" line 11, impact on 14 file(s)
    7 : "lctype.h" line 10, impact on 5 file(s)
    8 : "lapi.c" line 17, impact on 1 file(s)
    9 : "lauxlib.c" line 25, impact on 1 file(s)
    10 : "lbaselib.c" line 18, impact on 1 file(s)
    ... 10 of 44 details
...

```

**After:**

```
cppinclude --show_std_files=true

Start initialization project ...
Start analyze sources ...
Start report results ...
Most impact files:
1 : "assert.h" impact on 64 file(s)
Included by:
    1 : "luaconf.h" line 701, impact on 62 file(s)
    2 : "lutf8lib.c" line 13, impact on 1 file(s)
    3 : "ltests.h" line 24
    4 : "onelua.c" line 27
2 : "stddef.h" impact on 64 file(s)
Included by:
    1 : "luaconf.h" line 12, impact on 62 file(s)
    2 : "lua.h" line 13, impact on 61 file(s)
    3 : "llimits.h" line 12, impact on 40 file(s)
    4 : "lauxlib.h" line 12, impact on 18 file(s)
    5 : "lmem.h" line 11, impact on 15 file(s)
    6 : "ldebug.c" line 14, impact on 1 file(s)
    7 : "ldump.c" line 13, impact on 1 file(s)
    8 : "lfunc.c" line 13, impact on 1 file(s)
    9 : "linit.c" line 30, impact on 1 file(s)
    10 : "lmem.c" line 13, impact on 1 file(s)
    ... 10 of 15 details
...

```

## Fixed

### Fixed bug in multiline comments

Simple example:

```c++
/*
  first line
  second line #include "...".
*/
```

Real example: [gcc/gcc/incpath.c line: 398](https://github.com/gcc-mirror/gcc/blob/releases/gcc-10.2.0/gcc/incpath.c#L398)

```c++
/* Use given -I paths for #include "..." but not #include <...>, and
   don't search the directory of the present file for #include "...".
   (Note that -I. -I- is not the same as the default setup; -I. uses
   the compiler's working dir.)  */
```

**Before in output:** "..." isn't resolved

### Fixed bug in multiline string

Simple example:

```c++
" text\
 line #include "coretypes.h"\
\"
```

Real example: [gcc/genconditions.c line: 73](https://github.com/gcc-mirror/gcc/blob/releases/gcc-10.2.0/gcc/genconditions.c#L73)

```c++
puts ("\
...\
#include \"coretypes.h\"\n\
...\
");
```

**Before in output::** "coretypes.h" isn't resolved

### Fixed bug in strig with quotation marks

Simple example:

```c++
//main.cpp
std::string s = " \"#include <iostream> "
```

**Before:** File iostream was include by main.cpp

**After:** File iostream is ignored like string

