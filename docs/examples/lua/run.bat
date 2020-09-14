@ECHO OFF

set GIT_URL="https://github.com/lua/lua.git"
set TAG_NAME="v5.4.0"

call ..\tools\run_analyze_git_repo.bat %GIT_URL% %TAG_NAME%

