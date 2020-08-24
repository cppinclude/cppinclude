@ECHO OFF

set TAG_NAME="release/10.x"
set GIT_URL="https://github.com/llvm/llvm-project.git"

call ..\tools\run_analyze_git_repo.bat %TAG_NAME% %GIT_URL%

