@echo off
chcp 65001 >nul 2>&1
cd /d "%~dp0"

echo ========================================
echo   Week 6 Final Push to GitHub
echo ========================================
echo.

echo [1/4] Adding all changes...
git add -A
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to add files
    pause
    exit /b 1
)

echo.
echo [2/4] Creating commit...
git commit -m "feat: Complete Week 6 advanced search optimizations

- Implement AIStrategy interface for MinimaxEngine
- Add iterative deepening search with time management
- Implement Principal Variation Search (PVS)
- Add aspiration windows optimization
- Implement killer move heuristics
- Enhance move ordering system
- Integrate Week 6 tests into CMakeLists.txt
- Fix PVS zero-window logic
- Improve timeout handling
- Add boundary checks and error handling
- Optimize order_moves performance
- Update README with Week 6 completion status

All Week 6 tasks completed and tested.
Code quality: No linter errors, C++20 compliant.
Ready for production deployment."

if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to create commit
    pause
    exit /b 1
)

echo.
echo [3/4] Fetching remote changes...
git fetch origin
if %ERRORLEVEL% NEQ 0 (
    echo WARNING: Failed to fetch, continuing anyway...
)

echo.
echo [4/4] Pushing to GitHub...
git push origin main
if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo   [SUCCESS] Successfully pushed to GitHub!
    echo ========================================
) else (
    echo.
    echo ========================================
    echo   [FAIL] Push failed! Error code: %ERRORLEVEL%
    echo   Please check network connection and credentials
    echo ========================================
)

echo.
pause

