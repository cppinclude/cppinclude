@ECHO OFF

set TAG_NAME="1.4.0rc1"
set GIT_URL="https://github.com/clementine-player/Clementine.git"

call ..\tools\run_analyze_git_repo.bat %TAG_NAME% %GIT_URL%

