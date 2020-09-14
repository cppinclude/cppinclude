@ECHO OFF

set GIT_URL="https://github.com/clementine-player/Clementine.git"
set TAG_NAME="1.4.0rc1"

call ..\tools\run_analyze_git_repo.bat %GIT_URL% %TAG_NAME%

