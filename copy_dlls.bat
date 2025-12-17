@echo off
REM Copy MinGW runtime DLLs to project directory
REM COMP390 Honours Year Project (2025-26)

chcp 65001 >nul 2>&1

echo ========================================
echo   Copying MinGW Runtime DLLs
echo ========================================
echo.

REM Find GCC path
where g++ >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] g++ not found in PATH
    echo Please add MinGW to your PATH or manually copy DLLs
    pause
    exit /b 1
)

for /f "delims=" %%i in ('where g++') do set GCC_PATH=%%i
for %%i in ("%GCC_PATH%") do set MINGW_BIN=%%~dpi

echo GCC found at: %GCC_PATH%
echo MinGW bin directory: %MINGW_BIN%
echo.

REM Try to find DLLs in common locations
set DLL_COPIED=0

REM Check bin directory
if exist "%MINGW_BIN%libgcc_s_seh-1.dll" (
    copy /Y "%MINGW_BIN%libgcc_s_seh-1.dll" . >nul 2>&1
    if %ERRORLEVEL% EQU 0 (
        echo [OK] libgcc_s_seh-1.dll copied
        set DLL_COPIED=1
    )
)

if exist "%MINGW_BIN%libstdc++-6.dll" (
    copy /Y "%MINGW_BIN%libstdc++-6.dll" . >nul 2>&1
    if %ERRORLEVEL% EQU 0 (
        echo [OK] libstdc++-6.dll copied
    )
)

if exist "%MINGW_BIN%libwinpthread-1.dll" (
    copy /Y "%MINGW_BIN%libwinpthread-1.dll" . >nul 2>&1
    if %ERRORLEVEL% EQU 0 (
        echo [OK] libwinpthread-1.dll copied
    )
)

REM Check parent directories (for Scoop installations)
for %%i in ("%MINGW_BIN%") do set MINGW_PARENT=%%~dpi
if exist "%MINGW_PARENT%bin\libgcc_s_seh-1.dll" (
    copy /Y "%MINGW_PARENT%bin\libgcc_s_seh-1.dll" . >nul 2>&1
    if %ERRORLEVEL% EQU 0 (
        echo [OK] libgcc_s_seh-1.dll copied from parent
        set DLL_COPIED=1
    )
)

if exist "%MINGW_PARENT%bin\libstdc++-6.dll" (
    copy /Y "%MINGW_PARENT%bin\libstdc++-6.dll" . >nul 2>&1
)

if exist "%MINGW_PARENT%bin\libwinpthread-1.dll" (
    copy /Y "%MINGW_PARENT%bin\libwinpthread-1.dll" . >nul 2>&1
)

REM Check x86_64-w64-mingw32 subdirectory
if exist "%MINGW_BIN%..\x86_64-w64-mingw32\bin\libgcc_s_seh-1.dll" (
    copy /Y "%MINGW_BIN%..\x86_64-w64-mingw32\bin\libgcc_s_seh-1.dll" . >nul 2>&1
    if %ERRORLEVEL% EQU 0 (
        echo [OK] libgcc_s_seh-1.dll copied from x86_64-w64-mingw32
        set DLL_COPIED=1
    )
)

if %DLL_COPIED% EQU 0 (
    echo.
    echo [WARNING] libgcc_s_seh-1.dll not found automatically
    echo.
    echo Please manually copy the following DLLs from your MinGW installation:
    echo   - libgcc_s_seh-1.dll
    echo   - libstdc++-6.dll
    echo   - libwinpthread-1.dll
    echo.
    echo Common locations:
    echo   - C:\mingw64\bin
    echo   - C:\msys64\mingw64\bin
    echo   - D:\workspaces\Scoop\apps\gcc\*\*\bin
    echo.
) else (
    echo.
    echo [SUCCESS] All required DLLs copied!
    echo.
)

pause

