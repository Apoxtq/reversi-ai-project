@echo off
REM Set code page to UTF-8 for proper character display
chcp 65001 >nul 2>&1

REM ============================================
REM GitHub Push Script
REM Push local commits to GitHub repository
REM ============================================

echo.
echo ========================================
echo   Reversi AI - GitHub Push
echo ========================================
echo.

echo [1/3] Checking Git status...
git status --short
echo.

echo [2/3] Fetching remote changes...
git fetch origin
echo.

echo [3/3] Pushing to GitHub...
git push origin main
echo.

if %ERRORLEVEL% EQU 0 (
    echo ========================================
    echo   [SUCCESS] Successfully pushed to GitHub!
    echo   Repository: https://github.com/Apoxtq/reversi-ai-project
    echo ========================================
) else (
    echo ========================================
    echo   [FAIL] Push failed! Error code: %ERRORLEVEL%
    echo   Please check:
    echo   - Network connection
    echo   - GitHub credentials
    echo   - Remote conflicts
    echo ========================================
)

echo.
pause

