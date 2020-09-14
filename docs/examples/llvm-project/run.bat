@ECHO OFF

set GIT_URL="https://github.com/llvm/llvm-project.git"
set TAG_NAME="release/10.x"

call ..\tools\run_analyze_git_repo.bat %GIT_URL% %TAG_NAME%

