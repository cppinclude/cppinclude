source ../tools/examples_tools.sh

TAG_NAME="release/10.x"
GIT_URL="https://github.com/llvm/llvm-project.git"

run_analyze_git_repo $TAG_NAME $GIT_URL

