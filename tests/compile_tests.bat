@echo off
REM Compile all test programs
REM Usage: cd to project root and run "tests\compile_tests.bat"

echo ===============================================
echo Compiling Reversi Test Suite
echo ===============================================
echo.

REM Compile simple_test
echo [1/2] Compiling simple_test...
g++ -std=c++20 -O2 -I./src -o tests/simple_test.exe ^
    src/core/Board.cpp ^
    src/core/Move.cpp ^
    tests/simple_test.cpp

if %ERRORLEVEL% NEQ 0 (
    echo ERROR: simple_test compilation failed!
    exit /b 1
)
echo ✓ simple_test.exe created

echo.

REM Compile test_legal_moves
echo [2/3] Compiling test_legal_moves...
g++ -std=c++20 -O2 -I./src -o tests/test_legal_moves.exe ^
    src/core/Board.cpp ^
    src/core/Move.cpp ^
    tests/test_legal_moves.cpp

if %ERRORLEVEL% NEQ 0 (
    echo ERROR: test_legal_moves compilation failed!
    exit /b 1
)
echo ✓ test_legal_moves.exe created

echo.

REM Compile benchmark with maximum optimization
echo [3/3] Compiling benchmark (with -O3 optimization)...
g++ -std=c++20 -O3 -march=native -I./src -o tests/benchmark.exe ^
    src/core/Board.cpp ^
    src/core/Move.cpp ^
    tests/benchmark.cpp

if %ERRORLEVEL% NEQ 0 (
    echo ERROR: benchmark compilation failed!
    exit /b 1
)
echo ✓ benchmark.exe created

echo.
echo ===============================================
echo All tests compiled successfully!
echo ===============================================
echo.
echo Run tests:
echo   tests\simple_test.exe        - Basic functionality
echo   tests\test_legal_moves.exe   - Detailed move tests
echo   tests\benchmark.exe          - Performance benchmarks
echo.

