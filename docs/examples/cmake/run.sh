source ../tools/examples_tools.sh

GIT_URL="https://github.com/Kitware/CMake.git"
TAG_NAME="v3.19.2"
PROJECT_FOLDER="CMake"

run_analyze_git_repo_cmake $GIT_URL $TAG_NAME $PROJECT_FOLDER

