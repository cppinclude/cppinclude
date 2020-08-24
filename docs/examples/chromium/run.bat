@ECHO OFF

set TAG_NAME="86.0.4227.1"
set GIT_URL="https://github.com/chromium/chromium.git"

call ..\tools\run_analyze_git_repo.bat %TAG_NAME% %GIT_URL%

