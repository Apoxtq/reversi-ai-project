@echo off
echo ================================================
echo Week 3 - Minimax Engine Compilation
echo ================================================

echo.
echo [1/3] Configuring CMake...
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
if %errorlevel% neq 0 (
    echo ERROR: CMake configuration failed!
    pause
    exit /b 1
)

echo.
echo [2/3] Building project...
cmake --build build --config Release
if %errorlevel% neq 0 (
    echo ERROR: Build failed!
    pause
    exit /b 1
)

echo.
echo [3/3] Running tests...
echo.
echo --- Minimax Engine Test ---
.\build\bin\test_minimax.exe
if %errorlevel% neq 0 (
    echo WARNING: Some tests failed
)

echo.
echo ================================================
echo Build complete!
echo ================================================
echo.
echo Executables:
echo   - test_minimax.exe
echo   - reversi_ai.exe
echo.
pause

