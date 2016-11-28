cd deps
"C:\Program Files\Git\bin\git.exe" clone https://github.com/llvm-mirror/llvm
cd llvm
"C:\Program Files\Git\bin\git.exe" checkout release_37
mkdir build_vs14_x64_debug
mkdir build_vs14_x64_release

cd build_vs14_x64_debug
cmake -G"Visual Studio 14 Win64" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX:PATH=../install_vs14_x64_debug -DLLVM_TARGETS_TO_BUILD="X86" -DLLVM_BUILD_EXAMPLES:BOOL=FALSE ..
msbuild INSTALL.vcxproj /p:Configuration=Debug
cd ..

cd build_vs14_x64_release
cmake -G"Visual Studio 14 Win64" -DCMAKE_BUILD_TYPE=release -DCMAKE_INSTALL_PREFIX:PATH=../install_vs14_x64_release -DLLVM_TARGETS_TO_BUILD="X86" -DLLVM_BUILD_EXAMPLES:BOOL=FALSE ..
msbuild INSTALL.vcxproj /p:Configuration=Release
cd ..

cd ..\..