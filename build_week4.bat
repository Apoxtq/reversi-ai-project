@echo off
REM Set code page to UTF-8 for proper character display
chcp 65001 >nul 2>&1

REM Batch script to build Week 4 programs (Windows)
REM COMP390 Honours Year Project - Week 4

echo ========================================
echo   Building Week 4 Programs
echo   COMP390 - Advanced Evaluation
echo ========================================
echo.

set CXX=g++
set CXXFLAGS=-std=c++20 -O3 -Wall -Wextra -march=native
set INCLUDES=-Isrc

set CORE_SOURCES=src/core/Board.cpp
set AI_SOURCES_WEEK3=src/ai/Evaluator.cpp src/ai/MinimaxEngine.cpp
set AI_SOURCES_WEEK4=src/ai/StabilityAnalyzer.cpp src/ai/Evaluator_Week4.cpp

echo [1/3] Building test_stability...
%CXX% %CXXFLAGS% %INCLUDES% tests/test_stability.cpp %CORE_SOURCES% %AI_SOURCES_WEEK4% -o test_stability.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] test_stability.exe build failed!
    pause
    exit /b 1
)
echo [OK] test_stability.exe built
echo.

echo [2/3] Building self_play...
%CXX% %CXXFLAGS% %INCLUDES% src/research/self_play.cpp %CORE_SOURCES% %AI_SOURCES_WEEK3% %AI_SOURCES_WEEK4% -o self_play.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] self_play.exe build failed!
    pause
    exit /b 1
)
echo [OK] self_play.exe built
echo.

echo ========================================
echo   Build Complete
echo ========================================
echo.
echo To run tests:
echo   test_stability.exe
echo   self_play.exe 20
echo.

pause

