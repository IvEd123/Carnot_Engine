create solution: 
```
conan install . --build=missing
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/Debug/generators/conan_toolchain.cmake
```
