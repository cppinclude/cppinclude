@ECHO OFF

:git_clone
setlocal
set gitUrl=%1
set tagName=%2
git clone -b %gitUrl% --depth 1 %tagName%
endlocal

:run_analyze_git_repo
setlocal
set gitUrl=%1
set tagName=%2
set outPutFile="output_win.txt"
call :git_clone %gitUrl% %tagName%
cppinclude > %outPutFile%
type %outPutFile%
endlocal


