SETLOCAL

set gitUrl=%1%
set tagName=%2%

git clone -b %tagName% --depth 1 %gitUrl%

ENDLOCAL
