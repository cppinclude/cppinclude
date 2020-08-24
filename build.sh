cd build

cmake ../src/ -DCMAKE_BUILD_TYPE=Release && \
cmake --build . --config Release

cd -
cp build/cppinclude cppinclude && echo "File ./cppinclude compiled"

