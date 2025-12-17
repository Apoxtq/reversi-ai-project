@echo off
REM Compile and run UI Application (SFML Graphics)
REM COMP390 Honours Year Project (2025-26)

chcp 65001 >nul 2>&1

echo ========================================
echo   Reversi AI - UI Application (SFML)
echo ========================================
echo.

REM Check if SFML exists
if not exist "D:\SFML\SFML-2.5.1" (
    echo [ERROR] SFML not found at D:\SFML\SFML-2.5.1
    echo.
    echo Please install SFML 2.5+ or update the path in this script.
    echo.
    pause
    exit /b 1
)

echo SFML found. Compiling UI application...
echo.

REM Set SFML paths
set SFML_DIR=D:\SFML\SFML-2.5.1
set SFML_INCLUDE=%SFML_DIR%\include
set SFML_LIB=%SFML_DIR%\lib
set SFML_BIN=%SFML_DIR%\bin

REM Create build directory
if not exist build_temp mkdir build_temp

REM Compile core files
echo [1/8] Compiling core files...
g++ -std=c++20 -Wall -Wextra -O2 -c src/core/Board.cpp -o build_temp/Board.o -Isrc
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++20 -Wall -Wextra -O2 -c src/core/Move.cpp -o build_temp/Move.o -Isrc
if %ERRORLEVEL% NEQ 0 goto :error

REM Compile AI files
echo [2/8] Compiling AI engine...
g++ -std=c++20 -Wall -Wextra -O2 -c src/ai/Evaluator.cpp -o build_temp/Evaluator.o -Isrc
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++20 -Wall -Wextra -O2 -c src/ai/MinimaxEngine.cpp -o build_temp/MinimaxEngine.o -Isrc
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++20 -Wall -Wextra -O2 -c src/ai/TranspositionTable.cpp -o build_temp/TranspositionTable.o -Isrc
if %ERRORLEVEL% NEQ 0 goto :error

REM Compile UI files
echo [3/8] Compiling UI components...
g++ -std=c++20 -Wall -Wextra -O2 -c src/ui/UIStyle.cpp -o build_temp/UIStyle.o -Isrc -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++20 -Wall -Wextra -O2 -c src/ui/GameState.cpp -o build_temp/GameState.o -Isrc
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++20 -Wall -Wextra -O2 -c src/ui/UIComponent.cpp -o build_temp/UIComponent.o -Isrc -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++20 -Wall -Wextra -O2 -c src/ui/MenuSystem.cpp -o build_temp/MenuSystem.o -Isrc -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++20 -Wall -Wextra -O2 -c src/ui/BoardRenderer.cpp -o build_temp/BoardRenderer.o -Isrc -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++20 -Wall -Wextra -O2 -c src/ui/EventHandler.cpp -o build_temp/EventHandler.o -Isrc -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++20 -Wall -Wextra -O2 -c src/ui/AnimationSystem.cpp -o build_temp/AnimationSystem.o -Isrc -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++20 -Wall -Wextra -O2 -c src/ui/GameUI.cpp -o build_temp/GameUI.o -Isrc -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++20 -Wall -Wextra -O2 -c src/ui/NetworkLobbyState.cpp -o build_temp/NetworkLobbyState.o -Isrc -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error

REM Compile network files
echo [4/9] Compiling network module...
g++ -std=c++20 -Wall -Wextra -O2 -c src/network/NetworkProtocol.cpp -o build_temp/NetworkProtocol.o -Isrc -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++20 -Wall -Wextra -O2 -c src/network/TCPSocket.cpp -o build_temp/TCPSocket.o -Isrc -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++20 -Wall -Wextra -O2 -c src/network/RoomManager.cpp -o build_temp/RoomManager.o -Isrc -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++20 -Wall -Wextra -O2 -c src/network/NetworkGame.cpp -o build_temp/NetworkGame.o -Isrc -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error

REM Compile main UI
echo [5/9] Compiling main UI entry point...
g++ -std=c++20 -Wall -Wextra -O2 -c src/main_ui.cpp -o build_temp/main_ui.o -Isrc -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error

echo [6/9] Linking executable (static linking for MinGW runtime)...
g++ -std=c++20 -static-libgcc -static-libstdc++ -o reversi_ui_app.exe ^
    build_temp/Board.o ^
    build_temp/Move.o ^
    build_temp/Evaluator.o ^
    build_temp/MinimaxEngine.o ^
    build_temp/TranspositionTable.o ^
    build_temp/UIStyle.o ^
    build_temp/GameState.o ^
    build_temp/UIComponent.o ^
    build_temp/MenuSystem.o ^
    build_temp/BoardRenderer.o ^
    build_temp/EventHandler.o ^
    build_temp/AnimationSystem.o ^
    build_temp/GameUI.o ^
    build_temp/NetworkLobbyState.o ^
    build_temp/NetworkProtocol.o ^
    build_temp/TCPSocket.o ^
    build_temp/RoomManager.o ^
    build_temp/NetworkGame.o ^
    build_temp/main_ui.o ^
    -L%SFML_LIB% ^
    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network ^
    -lws2_32

if %ERRORLEVEL% NEQ 0 goto :error

echo.
echo [SUCCESS] Build completed!
echo Executable: reversi_ui_app.exe
echo.
echo ========================================
echo   Starting Reversi AI Platform...
echo ========================================
echo.
echo Note: Make sure SFML DLLs are in PATH or same directory as exe
echo.

REM Copy SFML DLLs if they exist (check bin directory first, then lib, check both naming conventions)
set SFML_DLLS_COPIED=0
if exist "%SFML_BIN%\sfml-graphics-2.dll" (
    echo Copying SFML DLLs from bin directory...
    copy /Y "%SFML_BIN%\sfml-graphics-2.dll" . >nul 2>&1
    copy /Y "%SFML_BIN%\sfml-window-2.dll" . >nul 2>&1
    copy /Y "%SFML_BIN%\sfml-system-2.dll" . >nul 2>&1
    copy /Y "%SFML_BIN%\sfml-network-2.dll" . >nul 2>&1
    if exist "%SFML_BIN%\sfml-audio-2.dll" copy /Y "%SFML_BIN%\sfml-audio-2.dll" . >nul 2>&1
    echo SFML DLLs copied.
    set SFML_DLLS_COPIED=1
) else if exist "%SFML_BIN%\libsfml-graphics-2.dll" (
    echo Copying SFML DLLs from bin directory...
    copy /Y "%SFML_BIN%\libsfml-graphics-2.dll" . >nul 2>&1
    copy /Y "%SFML_BIN%\libsfml-window-2.dll" . >nul 2>&1
    copy /Y "%SFML_BIN%\libsfml-system-2.dll" . >nul 2>&1
    copy /Y "%SFML_BIN%\libsfml-network-2.dll" . >nul 2>&1
    if exist "%SFML_BIN%\libsfml-audio-2.dll" copy /Y "%SFML_BIN%\libsfml-audio-2.dll" . >nul 2>&1
    echo SFML DLLs copied.
    set SFML_DLLS_COPIED=1
) else if exist "%SFML_LIB%\sfml-graphics-2.dll" (
    echo Copying SFML DLLs from lib directory...
    copy /Y "%SFML_LIB%\sfml-graphics-2.dll" . >nul 2>&1
    copy /Y "%SFML_LIB%\sfml-window-2.dll" . >nul 2>&1
    copy /Y "%SFML_LIB%\sfml-system-2.dll" . >nul 2>&1
    copy /Y "%SFML_LIB%\sfml-network-2.dll" . >nul 2>&1
    if exist "%SFML_LIB%\sfml-audio-2.dll" copy /Y "%SFML_LIB%\sfml-audio-2.dll" . >nul 2>&1
    echo SFML DLLs copied.
    set SFML_DLLS_COPIED=1
) else if exist "%SFML_LIB%\libsfml-graphics-2.dll" (
    echo Copying SFML DLLs from lib directory...
    copy /Y "%SFML_LIB%\libsfml-graphics-2.dll" . >nul 2>&1
    copy /Y "%SFML_LIB%\libsfml-window-2.dll" . >nul 2>&1
    copy /Y "%SFML_LIB%\libsfml-system-2.dll" . >nul 2>&1
    copy /Y "%SFML_LIB%\libsfml-network-2.dll" . >nul 2>&1
    if exist "%SFML_LIB%\libsfml-audio-2.dll" copy /Y "%SFML_LIB%\libsfml-audio-2.dll" . >nul 2>&1
    echo SFML DLLs copied.
    set SFML_DLLS_COPIED=1
)

if %SFML_DLLS_COPIED% EQU 0 (
    echo [WARNING] SFML DLLs not found! The program may not run correctly.
    echo Please ensure SFML DLLs are in PATH or same directory as exe.
)

REM Note: MinGW runtime DLLs are not needed due to static linking (-static-libgcc -static-libstdc++)
REM But we keep this section commented in case dynamic linking is needed in the future

reversi_ui_app.exe

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [WARNING] Program exited with error code: %ERRORLEVEL%
)

goto :end

:error
echo.
echo [ERROR] Compilation failed!
echo.
echo Troubleshooting:
echo 1. Make sure SFML is installed at D:\SFML\SFML-2.5.1
echo 2. Check that g++ compiler is in PATH
echo 3. Verify all source files exist
echo.
pause
exit /b 1

:end
pause

