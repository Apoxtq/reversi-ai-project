@echo off
REM Compile and run AI Demo (Human vs AI)
REM COMP390 Honours Year Project (2025-26)

chcp 65001 >nul 2>&1

echo ========================================
echo   Reversi AI - Human vs AI Game
echo ========================================
echo.

echo Compiling AI Demo...
echo.

REM Compile core files
g++ -std=c++20 -Wall -Wextra -O2 -c src/core/Board.cpp -o build_temp/Board.o -Isrc
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++20 -Wall -Wextra -O2 -c src/core/Move.cpp -o build_temp/Move.o -Isrc
if %ERRORLEVEL% NEQ 0 goto :error

REM Compile AI files
g++ -std=c++20 -Wall -Wextra -O2 -c src/ai/Evaluator.cpp -o build_temp/Evaluator.o -Isrc
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++20 -Wall -Wextra -O2 -c src/ai/MinimaxEngine.cpp -o build_temp/MinimaxEngine.o -Isrc
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++20 -Wall -Wextra -O2 -c src/ai/TranspositionTable.cpp -o build_temp/TranspositionTable.o -Isrc
if %ERRORLEVEL% NEQ 0 goto :error

REM Compile main demo
g++ -std=c++20 -Wall -Wextra -O2 -c src/ai_demo.cpp -o build_temp/ai_demo.o -Isrc
if %ERRORLEVEL% NEQ 0 goto :error

echo.
echo Linking executable...

REM Close any running instance
taskkill /F /IM ai_demo.exe >nul 2>&1
timeout /t 1 /nobreak >nul 2>&1
if exist ai_demo.exe del /F /Q ai_demo.exe >nul 2>&1

g++ -std=c++20 -static-libgcc -static-libstdc++ -o ai_demo.exe ^
    build_temp/Board.o ^
    build_temp/Move.o ^
    build_temp/Evaluator.o ^
    build_temp/MinimaxEngine.o ^
    build_temp/TranspositionTable.o ^
    build_temp/ai_demo.o

if %ERRORLEVEL% NEQ 0 goto :error

echo.
echo [SUCCESS] Build completed!
echo Executable: ai_demo.exe
echo.
echo ========================================
echo   Starting Human vs AI Game...
echo ========================================
echo.
ai_demo.exe

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [WARNING] Program exited with error code: %ERRORLEVEL%
)

goto :end

:error
echo.
echo [ERROR] Compilation failed!
pause
exit /b 1

:end
pause

