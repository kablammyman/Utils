echo export CC=/usr/bin/clang
echo export CXX=/usr/bin/clang++
mkdir -p build_linux_codeblocks
cd build_linux_codeblocks
cmake .. -G"CodeBlocks - Unix Makefiles"
cd ..
