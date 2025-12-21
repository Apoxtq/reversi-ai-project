@echo off
REM Build Reversi UI Application using CMake
REM COMP390 Honours Year Project (2025-26)

chcp 65001 >nul 2>&1

echo ========================================
echo   Reversi AI - CMake Build Script
echo ========================================
echo.

REM Check if CMake is available
where cmake >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] CMake not found in PATH
    echo.
    echo Please install CMake:
    echo   scoop install cmake
    echo   or download from https://cmake.org/download/
    echo.
    pause
    exit /b 1
)

echo CMake found. Setting up build...
echo.

REM Set SFML directory
set SFML_DIR=D:\SFML\SFML-2.5.1\lib\cmake\SFML

REM Create build directory
if not exist build mkdir build
cd build

REM Find make executable
set MAKE_PROGRAM=
for /f "delims=" %%i in ('powershell -Command "Get-Command make -ErrorAction SilentlyContinue | Select-Object -ExpandProperty Source"') do set "MAKE_PROGRAM=%%i"

if not defined MAKE_PROGRAM (
    echo [ERROR] make not found in PATH
    echo Please install: scoop install make
    pause
    exit /b 1
)

echo Found make: %MAKE_PROGRAM%

REM Configure CMake with MinGW Makefiles
echo [1/3] Configuring CMake...
echo Using MinGW Makefiles generator
cmake .. -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM="%MAKE_PROGRAM%" -DSFML_DIR=%SFML_DIR% -DCMAKE_BUILD_TYPE=Release

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERROR] CMake configuration failed!
    echo.
    echo Troubleshooting:
    echo 1. Make sure SFML is installed at D:\SFML\SFML-2.5.1
    echo 2. Check that CMake can find SFML
    echo 3. Install MinGW make: scoop install mingw
    echo 4. Try manual: cmake .. -G "MinGW Makefiles" -DSFML_DIR=%SFML_DIR%
    echo.
    cd ..
    pause
    exit /b 1
)

echo.
echo [2/3] Building project...
cmake --build . --config Release
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERROR] Build failed!
    cd ..
    pause
    exit /b 1
)

echo.
echo [3/3] Build completed successfully!
echo.
echo Executable location: build\bin\Release\reversi_ui_app.exe
echo.

REM Copy executable to project root for convenience
if exist "bin\Release\reversi_ui_app.exe" (
    copy /Y "bin\Release\reversi_ui_app.exe" ..\reversi_ui_app.exe >nul 2>&1
    echo Executable also copied to project root.
)

REM Copy SFML DLLs to project root if they exist in build directory
if exist "bin\Release\sfml-graphics-2.dll" (
    copy /Y "bin\Release\sfml-*.dll" ..\ >nul 2>&1
    copy /Y "bin\Release\openal32.dll" ..\ >nul 2>&1
    echo SFML DLLs copied to project root.
)

cd ..

echo.
echo ========================================
echo   Build Complete!
echo ========================================
echo.
echo To run the program:
echo   .\reversi_ui_app.exe
echo   or
echo   .\build\bin\Release\reversi_ui_app.exe
echo.
pause

