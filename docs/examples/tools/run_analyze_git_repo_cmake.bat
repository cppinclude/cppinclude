set gitUrl=%1%
set tagName=%2%
set projectFolder=%3%

echo %gitUrl% 
echo %tagName%

call ..\tools\run_cmake.bat %projectFolder%
call ..\tools\git_clone.bat %gitUrl% %tagName%
call ..\tools\run_analyze.bat
