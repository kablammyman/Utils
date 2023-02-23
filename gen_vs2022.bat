if not exist "build_vs2022" mkdir build_vs2022

cd build_vs2022

cmake -G "Visual Studio 17 2022" -A x64 ..

cd ..