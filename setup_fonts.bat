@echo off
chcp 65001 >nul 2>&1

echo ========================================
echo   Setup Fonts for Reversi AI
echo ========================================
echo.

cd /d "%~dp0"

REM Create font directories
echo [1/3] Creating font directories...
if not exist "build\bin\fonts" mkdir "build\bin\fonts"
if not exist "fonts" mkdir "fonts"
echo   Done.
echo.

echo [2/3] Copying Windows system fonts...
echo.

REM Copy commonly available Windows fonts
set "COPIED=0"

REM Arial (most common)
if exist "C:\Windows\Fonts\arial.ttf" (
    copy /Y "C:\Windows\Fonts\arial.ttf" "build\bin\fonts\" >nul 2>&1
    copy /Y "C:\Windows\Fonts\arial.ttf" "fonts\" >nul 2>&1
    echo   [OK] Arial (arial.ttf)
    set /A COPIED+=1
)

REM Segoe UI (Windows default)
if exist "C:\Windows\Fonts\segoeui.ttf" (
    copy /Y "C:\Windows\Fonts\segoeui.ttf" "build\bin\fonts\" >nul 2>&1
    copy /Y "C:\Windows\Fonts\segoeui.ttf" "fonts\" >nul 2>&1
    echo   [OK] Segoe UI (segoeui.ttf)
    set /A COPIED+=1
)

REM Consolas (monospace, good for UI)
if exist "C:\Windows\Fonts\consola.ttf" (
    copy /Y "C:\Windows\Fonts\consola.ttf" "build\bin\fonts\" >nul 2>&1
    copy /Y "C:\Windows\Fonts\consola.ttf" "fonts\" >nul 2>&1
    echo   [OK] Consolas (consola.ttf)
    set /A COPIED+=1
)

REM Create a Roboto-Regular.ttf symlink/copy from Arial
if exist "C:\Windows\Fonts\arial.ttf" (
    copy /Y "C:\Windows\Fonts\arial.ttf" "build\bin\fonts\Roboto-Regular.ttf" >nul 2>&1
    copy /Y "C:\Windows\Fonts\arial.ttf" "fonts\Roboto-Regular.ttf" >nul 2>&1
    echo   [OK] Roboto-Regular.ttf (using Arial)
)

echo.
echo [3/3] Verifying installation...
echo.

if exist "build\bin\fonts\Roboto-Regular.ttf" (
    echo   [OK] Main font installed: build\bin\fonts\Roboto-Regular.ttf
) else (
    echo   [WARNING] Main font not found!
)

echo.
echo ========================================
echo   Font Setup Complete!
echo ========================================
echo.
echo Fonts copied: %COPIED%
echo.
echo Available fonts in build\bin\fonts\:
dir /b "build\bin\fonts\" 2>nul
echo.
echo You can now restart the program to see the text!
echo.
pause






