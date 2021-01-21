SETLOCAL

set projectFolder=%1%
set back=%cd%

mkdir build
cd build

echo cmake ..\\%projectFolder% -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake ..\\%projectFolder% -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

cd ..

cd %back%

ENDLOCAL