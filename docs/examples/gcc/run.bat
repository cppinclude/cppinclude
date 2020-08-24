@ECHO OFF

set TAG_NAME="releases/gcc-10.2.0"
set GIT_URL="https://github.com/gcc-mirror/gcc.git"

call ..\tools\run_analyze_git_repo.bat %TAG_NAME% %GIT_URL%

