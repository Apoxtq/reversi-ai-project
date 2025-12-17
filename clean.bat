@echo off
REM Set code page to UTF-8 for proper character display
chcp 65001 >nul 2>&1

REM Clean script for Reversi AI Project
REM Removes all build artifacts and temporary files

echo ========================================
echo   Reversi AI - Clean Script
echo ========================================
echo.

echo Cleaning build artifacts...

REM Remove build directories
if exist build (
    echo - Removing build/
    rmdir /s /q build
)

if exist build_temp (
    echo - Removing build_temp/
    rmdir /s /q build_temp
)

REM Remove executables
if exist reversi_ai.exe (
    echo - Removing reversi_ai.exe
    del /q reversi_ai.exe
)

if exist ai_demo.exe (
    echo - Removing ai_demo.exe
    del /q ai_demo.exe
)

if exist reversi_ui_app.exe (
    echo - Removing reversi_ui_app.exe
    del /q reversi_ui_app.exe
)

REM Remove MinGW runtime DLLs (not needed if using static linking)
if exist libgcc_s_seh-1.dll (
    echo - Removing libgcc_s_seh-1.dll
    del /q libgcc_s_seh-1.dll
)

if exist libstdc++-6.dll (
    echo - Removing libstdc++-6.dll
    del /q libstdc++-6.dll
)

if exist libwinpthread-1.dll (
    echo - Removing libwinpthread-1.dll
    del /q libwinpthread-1.dll
)

REM Remove object files
if exist *.o (
    echo - Removing *.o files
    del /q *.o
)

REM Remove temporary files
if exist *.log (
    echo - Removing *.log files
    del /q *.log
)

if exist *.txt (
    if exist output.txt del /q output.txt
    if exist cmake_output.txt del /q cmake_output.txt
)

REM Remove CMake cache files in root (if built in-source by mistake)
if exist CMakeCache.txt (
    echo - Removing CMakeCache.txt
    del /q CMakeCache.txt
)

if exist CMakeFiles (
    echo - Removing CMakeFiles/
    rmdir /s /q CMakeFiles
)

if exist cmake_install.cmake (
    echo - Removing cmake_install.cmake
    del /q cmake_install.cmake
)

if exist Makefile (
    echo - Removing Makefile
    del /q Makefile
)

echo.
echo [SUCCESS] Clean completed!
echo All build artifacts have been removed.
echo.

pause

