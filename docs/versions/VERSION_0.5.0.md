# VERSION 0.5.0

## Added

### New argument show_details

Show or hide details of reports. Example [lua](docs/examples/lua) :

**Before:**

```
cppinclude --report_details_limit=3
...
Most impact files:
1 : "luaconf.h" impact on 62 files
Included by:
	1 : "lua.h" line 16, impact on 61 files
	2 : "onelua.c" line 49
2 : "lua.h" impact on 61 files
Included by:
	1 : "llimits.h" line 15, impact on 40 files
	2 : "lobject.h" line 16, impact on 34 files
	3 : "lstate.h" line 10, impact on 26 files
	... 3 of 44 details
3 : "llimits.h" impact on 40 files
Included by:
	1 : "lobject.h" line 15, impact on 34 files
	2 : "lmem.h" line 13, impact on 15 files
	3 : "lopcodes.h" line 10, impact on 9 files
	... 3 of 8 details
4 : "lprefix.h" impact on 35 files
Included by:
	1 : "lapi.c" line 10, impact on 1 file
	2 : "lauxlib.c" line 10, impact on 1 file
	3 : "lbaselib.c" line 10, impact on 1 file
	... 3 of 35 details
5 : "lobject.h" impact on 34 files
...

```

**After:**

```
cppinclude --show_details=false --report_details_limit=3
...
Most impact files:
1 : "luaconf.h" impact on 62 files
2 : "lua.h" impact on 61 files
3 : "llimits.h" impact on 40 files
4 : "lprefix.h" impact on 35 files
5 : "lobject.h" impact on 34 files
...

```

### New argument thousands_separator

Set thousands separator. Example :

**Before:**

```
cppinclude
...
Most impact files:
1 : "supper_header.h" impact on 123456 files
2 : "config.h" impact on 12345 files
3 : "limits.h" impact on 1234 files
...

```

**After:**

```
cppinclude --thousands_separator=,
...
Most impact files:
1 : "supper_header.h" impact on 123,456 files
2 : "config.h" impact on 12,345 files
3 : "limits.h" impact on 1,234 files
...

```

### New argument verbose_ignore

Show ignored files and foldes

```
cppinclude --verbose_ignore

Path "sys/types.h" was ignored by "sys/.*.h"
Path "mach/mach_init.h" was ignored by "mach/.*.h"
Folder ".../3rdparty" was ignored
...

```


