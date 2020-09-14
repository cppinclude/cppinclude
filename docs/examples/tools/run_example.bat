SETLOCAL
set exampleDir=%1%

if "%exampleDir%" NEQ ".\tools" (
	set back=%cd%
	cd %exampleDir%
	call .\run.bat
	cd %back%
	call .\tools\clean_example_dir.bat %exampleDir%
)
ENDLOCAL