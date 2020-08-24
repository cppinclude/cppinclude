@ECHO OFF

set TAG_NAME="4.0.0-dev"
set GIT_URL="https://github.com/videolan/vlc.git"

call ..\tools\run_analyze_git_repo.bat %TAG_NAME% %GIT_URL%

