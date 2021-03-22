tempDir=${TMPDIR:-/tmp}/cppinclude

echo "Prepare sources..."
[[ -d $tempDir ]] && rm -r $tempDir
mkdir -p $tempDir && echo "Created new folder $tempDir"

cp .clang-tidy $tempDir && echo "Copied .clang-tidy"
cp -r ./src $tempDir  && echo "Copied src folder"

echo "Run clang-format..."
find $tempDir -regex '.*\.\(cpp\|hpp\)' ! -path '$tempDir/src/3rd-part/**' -exec clang-format -style=file -i {} \;

echo "Run meld..."
meld $tempDir/src ./src
