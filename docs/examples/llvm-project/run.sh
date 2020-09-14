source ../tools/examples_tools.sh

GIT_URL="https://github.com/llvm/llvm-project.git"
TAG_NAME="release/10.x"

run_analyze_git_repo $GIT_URL $TAG_NAME

