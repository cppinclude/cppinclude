@ECHO OFF

set TAG_NAME="v7.8.9"
set GIT_URL="https://github.com/notepad-plus-plus/notepad-plus-plus.git"

call ..\tools\run_analyze_git_repo.bat %TAG_NAME% %GIT_URL%

