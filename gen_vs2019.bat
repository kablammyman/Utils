if not exist "build_vs2019" mkdir build_vs2019

cd build_vs2019

cmake -G "Visual Studio 16 2019" -A x64 ..

cd ..