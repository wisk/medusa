cd deps
"C:\Program Files\Git\bin\git.exe" clone https://github.com/llvm-mirror/llvm
cd llvm
"C:\Program Files\Git\bin\git.exe" checkout release_60
mkdir build_vs15_x64_debug
mkdir build_vs15_x64_release

cd build_vs15_x64_debug
cmake -Thost=x64 -G"Visual Studio 15 Win64" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX:PATH=../install_vs15_x64_debug -DLLVM_TARGETS_TO_BUILD="X86" -DLLVM_BUILD_EXAMPLES:BOOL=FALSE ..
cmake --build . --config debug --target install
cd ..

cd build_vs15_x64_release
cmake -Thost=x64 -G"Visual Studio 15 Win64" -DCMAKE_BUILD_TYPE=release -DCMAKE_INSTALL_PREFIX:PATH=../install_vs15_x64_release -DLLVM_TARGETS_TO_BUILD="X86" -DLLVM_BUILD_EXAMPLES:BOOL=FALSE ..
cmake --build . --config release --target install
cd ..

cd ..\..