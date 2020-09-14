set gitUrl=%1%
set tagName=%2%

echo %gitUrl% 
echo %tagName%

call ..\tools\git_clone.bat %gitUrl% %tagName%
call ..\tools\run_analyze.bat
