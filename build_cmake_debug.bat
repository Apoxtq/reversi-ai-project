@echo off
REM Build Reversi UI Application using CMake (Debug mode)
REM COMP390 Honours Year Project (2025-26)

chcp 65001 >nul 2>&1

echo ========================================
echo   Reversi AI - CMake Build (Debug)
echo ========================================
echo.

REM Check if CMake is available
where cmake >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] CMake not found in PATH
    pause
    exit /b 1
)

REM Set SFML directory
set SFML_DIR=D:\SFML\SFML-2.5.1\lib\cmake\SFML

REM Create build directory
if not exist build mkdir build
cd build

REM Configure CMake (Debug)
echo [1/3] Configuring CMake (Debug)...
cmake .. -DSFML_DIR=%SFML_DIR% -DCMAKE_BUILD_TYPE=Debug
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] CMake configuration failed!
    cd ..
    pause
    exit /b 1
)

echo.
echo [2/3] Building project (Debug)...
cmake --build . --config Debug
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Build failed!
    cd ..
    pause
    exit /b 1
)

echo.
echo [3/3] Debug build completed!
echo.
echo Executable: build\bin\Debug\reversi_ui_app.exe
echo.

cd ..
pause






