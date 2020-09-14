SETLOCAL
set exampleDir=%1%

set back=%cd%
cd %exampleDir%
for /d %%i in (.\*) do (
	rd /s /q %%i
)
cd %back%
ENDLOCAL