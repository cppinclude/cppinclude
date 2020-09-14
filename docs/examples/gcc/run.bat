@ECHO OFF

set GIT_URL="https://github.com/gcc-mirror/gcc.git"
set TAG_NAME="releases/gcc-10.2.0"

call ..\tools\run_analyze_git_repo.bat %GIT_URL% %TAG_NAME%

