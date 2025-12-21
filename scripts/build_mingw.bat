@echo off
REM Build script for MinGW (Ninja) - run from project root
SETLOCAL
set PROJ_ROOT=%~dp0..
cd /d "%PROJ_ROOT%"

REM Create build directory
if not exist build_mingw mkdir build_mingw
cd build_mingw

REM Configure with Ninja using MinGW-w64 toolchain
cmake -G Ninja -DCMAKE_C_COMPILER=D:/Software/mingw64/bin/gcc.exe -DCMAKE_CXX_COMPILER=D:/Software/mingw64/bin/g++.exe -DCMAKE_BUILD_TYPE=Release ..
if ERRORLEVEL 1 (
  echo CMake configuration failed
  exit /b 1
)

REM Build
ninja -j %NUMBER_OF_PROCESSORS%
if ERRORLEVEL 1 (
  echo Build failed
  exit /b 2
)

REM Run unit tests
ctest --output-on-failure -C Release
if ERRORLEVEL 1 (
  echo Some tests failed
  exit /b 3
)

echo Build and tests successful. Binaries are in build_mingw/bin
ENDLOCAL

