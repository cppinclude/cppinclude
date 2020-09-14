@ECHO OFF

set GIT_URL="https://github.com/videolan/vlc.git"
set TAG_NAME="4.0.0-dev"

call ..\tools\run_analyze_git_repo.bat %GIT_URL% %TAG_NAME%

