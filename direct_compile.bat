@echo off
REM Set code page to UTF-8 for proper character display
chcp 65001 >nul 2>&1

echo ================================================
echo Week 3 - Direct G++ Compilation
echo ================================================
echo.

REM Create output directories
if not exist "bin" mkdir bin
if not exist "obj" mkdir obj

echo [1/4] Compiling core modules...
echo   - Board.cpp
g++ -c src/core/Board.cpp -o obj/Board.o -Isrc -std=c++20 -O3 -Wall -Wextra
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to compile Board.cpp
    pause
    exit /b 1
)

echo   - Move.cpp
g++ -c src/core/Move.cpp -o obj/Move.o -Isrc -std=c++20 -O3 -Wall -Wextra
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to compile Move.cpp
    pause
    exit /b 1
)

echo.
echo [2/4] Compiling AI modules...
echo   - Evaluator.cpp
g++ -c src/ai/Evaluator.cpp -o obj/Evaluator.o -Isrc -std=c++20 -O3 -Wall -Wextra
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to compile Evaluator.cpp
    pause
    exit /b 1
)

echo   - MinimaxEngine.cpp
g++ -c src/ai/MinimaxEngine.cpp -o obj/MinimaxEngine.o -Isrc -std=c++20 -O3 -Wall -Wextra
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to compile MinimaxEngine.cpp
    pause
    exit /b 1
)

echo.
echo [3/4] Compiling test program...
g++ -c tests/test_minimax.cpp -o obj/test_minimax.o -Isrc -Itests -std=c++20 -O3 -Wall -Wextra
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to compile test_minimax.cpp
    pause
    exit /b 1
)

echo.
echo [4/4] Linking test executable...
g++ obj/Board.o obj/Move.o obj/Evaluator.o obj/MinimaxEngine.o obj/test_minimax.o -o bin/test_minimax.exe -std=c++20
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to link test_minimax.exe
    pause
    exit /b 1
)

echo.
echo ================================================
echo [SUCCESS] Build Success!
echo ================================================
echo.
echo Running tests...
echo.
bin\test_minimax.exe

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [WARNING] Tests exited with error code: %ERRORLEVEL%
)

echo.
echo ================================================
echo Test Complete!
echo ================================================
pause

