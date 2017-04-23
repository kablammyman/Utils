if not exist "build_vs2017" mkdir build_vs2017

cd build_vs2017

cmake -G "Visual Studio 15 2017 Win64" ..

cd ..