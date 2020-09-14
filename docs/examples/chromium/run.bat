@ECHO OFF

set GIT_URL="https://github.com/chromium/chromium.git"
set TAG_NAME="86.0.4227.1"

call ..\tools\run_analyze_git_repo.bat %GIT_URL% %TAG_NAME%

