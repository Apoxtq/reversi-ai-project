@echo off
chcp 65001 >nul
echo.
echo ========================================
echo   📤 GitHub 文档上传工具
echo ========================================
echo.
echo 这个工具会将选定的文档从 project_docs/github/ 复制到根目录
echo 以便提交到 GitHub 仓库
echo.
echo ========================================
echo   选择要复制的文档：
echo ========================================
echo.
echo [1] 全部文档 (5个)
echo     - README.md
echo     - GETTING_STARTED.md
echo     - BUILD_AND_RUN.md
echo     - QUICK_REFERENCE.md
echo     - PROJECT_SETUP_SUMMARY.md
echo.
echo [2] 必传文档 (3个) ⭐ 推荐
echo     - README.md
echo     - GETTING_STARTED.md
echo     - BUILD_AND_RUN.md
echo.
echo [3] 仅 README.md
echo.
echo [4] 自定义选择
echo.
echo [0] 取消
echo.
echo ========================================
set /p choice="请选择 (0-4): "

if "%choice%"=="0" (
    echo.
    echo 已取消。
    goto :end
)

if "%choice%"=="1" (
    echo.
    echo 📋 复制全部文档...
    copy github\README.md ..\README.md
    copy github\GETTING_STARTED.md ..\GETTING_STARTED.md
    copy github\BUILD_AND_RUN.md ..\BUILD_AND_RUN.md
    copy github\QUICK_REFERENCE.md ..\QUICK_REFERENCE.md
    copy github\PROJECT_SETUP_SUMMARY.md ..\PROJECT_SETUP_SUMMARY.md
    goto :success
)

if "%choice%"=="2" (
    echo.
    echo 📋 复制必传文档...
    copy github\README.md ..\README.md
    copy github\GETTING_STARTED.md ..\GETTING_STARTED.md
    copy github\BUILD_AND_RUN.md ..\BUILD_AND_RUN.md
    goto :success
)

if "%choice%"=="3" (
    echo.
    echo 📋 复制 README.md...
    copy github\README.md ..\README.md
    goto :success
)

if "%choice%"=="4" (
    echo.
    echo 📋 自定义选择
    echo.
    set /p readme="复制 README.md? (y/n): "
    if /i "%readme%"=="y" copy github\README.md ..\README.md
    
    set /p getting="复制 GETTING_STARTED.md? (y/n): "
    if /i "%getting%"=="y" copy github\GETTING_STARTED.md ..\GETTING_STARTED.md
    
    set /p build="复制 BUILD_AND_RUN.md? (y/n): "
    if /i "%build%"=="y" copy github\BUILD_AND_RUN.md ..\BUILD_AND_RUN.md
    
    set /p quick="复制 QUICK_REFERENCE.md? (y/n): "
    if /i "%quick%"=="y" copy github\QUICK_REFERENCE.md ..\QUICK_REFERENCE.md
    
    set /p setup="复制 PROJECT_SETUP_SUMMARY.md? (y/n): "
    if /i "%setup%"=="y" copy github\PROJECT_SETUP_SUMMARY.md ..\PROJECT_SETUP_SUMMARY.md
    
    goto :success
)

echo.
echo ❌ 无效的选择
goto :end

:success
echo.
echo ========================================
echo   ✅ 文档复制完成！
echo ========================================
echo.
echo 📋 后续步骤：
echo.
echo   1. 查看复制的文件：
echo      cd ..
echo      dir *.md
echo.
echo   2. 提交到 Git：
echo      git add README.md GETTING_STARTED.md ...
echo      git commit -m "docs: update documentation"
echo      git push
echo.
echo ========================================

:end
echo.
pause
