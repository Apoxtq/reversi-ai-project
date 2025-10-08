@echo off
REM Build script for Reversi AI Project
REM COMP390 Honours Year Project (2025-26)

echo ========================================
echo   Reversi AI - Build Script
echo ========================================
echo.

REM Check for MinGW
where mingw32-make >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo [INFO] MinGW not found. Trying to use Visual Studio or other generator...
    
    REM Try to find Visual Studio
    where cl >nul 2>nul
    if %ERRORLEVEL% NEQ 0 (
        echo [ERROR] No suitable compiler found!
        echo [INFO] Please install one of the following:
        echo   - MinGW: scoop install mingw
        echo   - Visual Studio (with C++ tools)
        pause
        exit /b 1
    )
)

echo [STEP 1] Cleaning old build files...
if exist build rmdir /s /q build
mkdir build
cd build

echo.
echo [STEP 2] Configuring CMake project...
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] CMake configuration failed!
    cd ..
    pause
    exit /b 1
)

echo.
echo [STEP 3] Building project...
cmake --build . --config Release

if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Build failed!
    cd ..
    pause
    exit /b 1
)

echo.
echo [SUCCESS] Build completed!
echo Executable location: build\bin\reversi_ai.exe
echo.

cd ..
pause

