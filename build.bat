cd build

cmake ..\src\ -DCMAKE_BUILD_TYPE=Release && cmake --build . --config Release

cd ..
copy build\Release\cppinclude.exe cppinclude.exe && echo "File cppinclude.exe compiled"
