call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\vcvars32.bat"
devenv.exe "Utils.sln" /build "Debug|x86"
devenv.exe "Utils.sln" /build "Debug|x64"
devenv.exe "Utils.sln" /build "Release|x86"
devenv.exe "Utils.sln" /build "Release|x64"
devenv.exe "Utils.sln" /build "vis studio 2010 debug|x86"
devenv.exe "Utils.sln" /build "vis studio 2010 release|x86"


pause