echo export CC=/usr/bin/clang
echo export CXX=/usr/bin/clang++
mkdir -p build_linux
mkdir -p x64/Debug
mkdir -p x64/Release
cd "build_linux"
cmake -DCMAKE_BUILD_TYPE=Debug ..
#cmake -DCMAKE_BUILD_TYPE=Release ".."
#cmake ".."
cd ..