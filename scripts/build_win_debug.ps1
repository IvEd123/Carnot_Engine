$ErrorActionPreference = 'Stop'

$RootDir = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$BuildType = 'Debug'
$OutDir = Join-Path $RootDir 'build'

conan install $RootDir --output-folder=$OutDir --build=missing -s build_type=$BuildType `
  -c tools.cmake.cmaketoolchain:extra_variables='{"CMAKE_POLICY_VERSION_MINIMUM":"3.5"}'

$BuildDir = Join-Path $OutDir "build/$BuildType"
$Toolchain = Join-Path $BuildDir 'generators/conan_toolchain.cmake'

cmake -S $RootDir -B $BuildDir -DCMAKE_TOOLCHAIN_FILE=$Toolchain -DCMAKE_BUILD_TYPE=$BuildType -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build $BuildDir
