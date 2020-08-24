source ../tools/examples_tools.sh

TAG_NAME="releases/gcc-10.2.0"
GIT_URL="https://github.com/gcc-mirror/gcc.git"

run_analyze_git_repo $TAG_NAME $GIT_URL

