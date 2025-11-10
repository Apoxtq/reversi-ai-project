@echo off
REM Set code page to UTF-8 for proper character display
chcp 65001 >nul 2>&1

echo ================================================
echo Week 5 - Transposition Table Test Compilation
echo ================================================
echo.

REM Create output directory
if not exist "tests" mkdir tests

echo [1/2] Compiling test_transposition...
g++ -std=c++20 -O2 -I./src -I./tests -o tests/test_transposition.exe ^
    src/core/Board.cpp ^
    src/core/Move.cpp ^
    src/ai/Evaluator.cpp ^
    src/ai/MinimaxEngine.cpp ^
    src/ai/TranspositionTable.cpp ^
    tests/test_transposition.cpp

if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] test_transposition compilation failed!
    pause
    exit /b 1
)
echo [OK] test_transposition.exe created
echo.

echo [2/2] Running tests...
echo.
tests\test_transposition.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ================================================
    echo [SUCCESS] All tests passed!
    echo ================================================
) else (
    echo.
    echo ================================================
    echo [FAIL] Some tests failed
    echo ================================================
)

pause

