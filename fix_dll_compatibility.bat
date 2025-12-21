@echo off
chcp 65001 >nul 2>&1

echo ========================================
echo   Fix DLL Compatibility Issue
echo ========================================
echo.

echo Problem: SFML 2.5.1 was compiled with an older GCC version (likely 7.x)
echo Current DLLs: GCC 13.2.0 (incompatible C++ ABI)
echo.

cd /d "%~dp0"

REM Clean up incompatible DLLs
echo [1/3] Removing incompatible DLLs...
del /Q "build\bin\libgcc_s_seh-1.dll" 2>nul
del /Q "build\bin\libstdc++-6.dll" 2>nul
del /Q "build\bin\libwinpthread-1.dll" 2>nul
echo   Done.
echo.

REM Create temp directory
set "TEMP_DIR=%TEMP%\mingw_old_%RANDOM%"
mkdir "%TEMP_DIR%" 2>nul

echo [2/3] Downloading MinGW GCC 7.3.0 runtime (compatible with SFML 2.5.1)...
echo.

REM Download MinGW 7.3.0 which is compatible with SFML 2.5.1
curl -L "https://sourceforge.net/projects/mingw-w64/files/Toolchains%%20targetting%%20Win64/Personal%%20Builds/mingw-builds/7.3.0/threads-posix/seh/x86_64-7.3.0-release-posix-seh-rt_v5-rev0.7z/download" -o "%TEMP_DIR%\mingw73.7z"

if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Download failed!
    echo.
    echo Alternative solution: Download SFML precompiled with all DLLs
    echo.
    goto :alternative
)

echo [3/3] Extracting compatible DLLs...
echo.

REM Check for 7-Zip
set "SEVENZIP="
if exist "C:\Program Files\7-Zip\7z.exe" set "SEVENZIP=C:\Program Files\7-Zip\7z.exe"
if exist "C:\Program Files (x86)\7-Zip\7z.exe" set "SEVENZIP=C:\Program Files (x86)\7-Zip\7z.exe"

if not defined SEVENZIP (
    echo 7-Zip not found. Trying alternative method...
    tar -xf "%TEMP_DIR%\mingw73.7z" -C "%TEMP_DIR%" mingw64/bin/libgcc_s_seh-1.dll mingw64/bin/libstdc++-6.dll mingw64/bin/libwinpthread-1.dll 2>nul
    if %ERRORLEVEL% NEQ 0 goto :alternative
) else (
    "%SEVENZIP%" x "%TEMP_DIR%\mingw73.7z" -o"%TEMP_DIR%" mingw64\bin\libgcc_s_seh-1.dll mingw64\bin\libstdc++-6.dll mingw64\bin\libwinpthread-1.dll -y >nul
    if %ERRORLEVEL% NEQ 0 goto :alternative
)

REM Copy DLLs
if exist "%TEMP_DIR%\mingw64\bin\libgcc_s_seh-1.dll" (
    copy /Y "%TEMP_DIR%\mingw64\bin\*.dll" "build\bin\" >nul
    echo   [OK] Compatible DLLs installed!
    echo.
    echo ========================================
    echo   SUCCESS!
    echo ========================================
    echo.
    echo You can now run: build\bin\reversi_ui_app.exe
    goto :cleanup
)

:alternative
echo.
echo ========================================
echo   Alternative Solution
echo ========================================
echo.
echo The automatic fix failed. Here are two solutions:
echo.
echo **SOLUTION A: Use MSVC version of SFML (Recommended)**
echo.
echo 1. Download MSVC SFML from: https://www.sfml-dev.org/download/sfml/2.5.1/
echo    Choose: SFML-2.5.1-windows-vc15-64-bit.zip
echo 2. Extract to D:\SFML\SFML-2.5.1-VC15\
echo 3. Update CMakeLists.txt:
echo    set(SFML_DIR "D:/SFML/SFML-2.5.1-VC15/lib/cmake/SFML")
echo 4. Install Visual Studio or Build Tools
echo 5. Rebuild with: cmake -G "Visual Studio 17 2022" ..
echo.
echo **SOLUTION B: Manual DLL download**
echo.
echo 1. Download: https://sourceforge.net/projects/mingw-w64/files/Toolchains%%20targetting%%20Win64/Personal%%20Builds/mingw-builds/7.3.0/threads-posix/seh/x86_64-7.3.0-release-posix-seh-rt_v5-rev0.7z
echo 2. Extract and copy these 3 DLLs from mingw64\bin\ to build\bin\:
echo    - libgcc_s_seh-1.dll
echo    - libstdc++-6.dll
echo    - libwinpthread-1.dll
echo.

:cleanup
if exist "%TEMP_DIR%" (
    rd /s /q "%TEMP_DIR%" 2>nul
)

pause






