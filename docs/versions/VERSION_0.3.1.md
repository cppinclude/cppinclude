# VERSION 0.3.1

## Fixed

### Fixed detect file name after comments

Example:

```c++
#include /*<text1>*/ "filename1"
#include /*"text2"*/ <filename2>


```

**Before:** File text1 and text2 were file names

**After:** filename1 and filename2 are file names

