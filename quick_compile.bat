@echo off
REM Quick compile script for testing (without CMake)
REM COMP390 Honours Year Project (2025-26)

echo ========================================
echo   Reversi AI - Quick Compile
echo ========================================
echo.

echo Compiling source files...

g++ -std=c++20 -Wall -Wextra -O2 ^
    -c src/core/Board.cpp -o build_temp/Board.o -Isrc

g++ -std=c++20 -Wall -Wextra -O2 ^
    -c src/core/Move.cpp -o build_temp/Move.o -Isrc

g++ -std=c++20 -Wall -Wextra -O2 ^
    -c src/research/benchmark/Benchmark.cpp -o build_temp/Benchmark.o -Isrc

g++ -std=c++20 -Wall -Wextra -O2 ^
    -c src/main.cpp -o build_temp/main.o -Isrc

echo.
echo Linking executable...

g++ -std=c++20 -o reversi_ai.exe ^
    build_temp/Board.o ^
    build_temp/Move.o ^
    build_temp/Benchmark.o ^
    build_temp/main.o

if %ERRORLEVEL% EQU 0 (
    echo.
    echo [SUCCESS] Build completed!
    echo Executable: reversi_ai.exe
    echo.
    echo Running program...
    echo.
    reversi_ai.exe
) else (
    echo [ERROR] Linking failed!
    pause
    exit /b 1
)

pause

