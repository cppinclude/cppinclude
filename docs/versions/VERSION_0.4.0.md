# VERSION 0.4.0

## Added

### New argument show_only_std_headers

Show only standard library headers in output. Example [lua](docs/examples/lua) :

**Before:**

```
cppinclude --report=most_impact  --show_only_std_headers=false --ignore_system_includes=false --report_details_limit=3

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
	... 3 of 44 details
3 : "llimits.h" impact on 40 file(s)
Included by:
	1 : "lobject.h" line 15, impact on 34 file(s)
	2 : "lmem.h" line 13, impact on 15 file(s)
	3 : "lopcodes.h" line 10, impact on 9 file(s)
	... 3 of 8 details
4 : "lprefix.h" impact on 35 file(s)
Included by:
	1 : "lapi.c" line 10, impact on 1 file(s)
	2 : "lauxlib.c" line 10, impact on 1 file(s)
	3 : "lbaselib.c" line 10, impact on 1 file(s)
	... 3 of 35 details
...

```

**After:**

```
cppinclude --report=most_impact  --show_only_std_headers=false --ignore_system_includes=false --report_details_limit=3

Start initialization project ...
Start analyze sources ...
Start report results ...
Most impact files:
1 : "assert.h" impact on 64 file(s)
Included by:
	1 : "luaconf.h" line 701, impact on 62 file(s)
	2 : "lutf8lib.c" line 13, impact on 1 file(s)
	3 : "ltests.h" line 24
	... 3 of 4 details
2 : "stddef.h" impact on 64 file(s)
Included by:
	1 : "luaconf.h" line 12, impact on 62 file(s)
	2 : "lua.h" line 13, impact on 61 file(s)
	3 : "llimits.h" line 12, impact on 40 file(s)
	... 3 of 15 details
3 : "limits.h" impact on 63 file(s)
Included by:
	1 : "luaconf.h" line 11, impact on 62 file(s)
	2 : "llimits.h" line 11, impact on 40 file(s)
	3 : "lctype.h" line 34, impact on 5 file(s)
	... 3 of 16 details
4 : "stdint.h" impact on 63 file(s)
Included by:
	1 : "luaconf.h" line 660, impact on 62 file(s)
...

```

