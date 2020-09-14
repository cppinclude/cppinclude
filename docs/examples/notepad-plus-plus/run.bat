@ECHO OFF

set GIT_URL="https://github.com/notepad-plus-plus/notepad-plus-plus.git"
set TAG_NAME="v7.8.9"

call ..\tools\run_analyze_git_repo.bat %GIT_URL% %TAG_NAME% 

