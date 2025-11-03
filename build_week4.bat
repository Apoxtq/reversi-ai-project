@echo off
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
if %errorlevel% == 0 (
    echo [OK] test_stability.exe built
) else (
    echo [FAIL] test_stability.exe build failed
)
echo.

echo [2/3] Building self_play...
%CXX% %CXXFLAGS% %INCLUDES% src/research/self_play.cpp %CORE_SOURCES% %AI_SOURCES_WEEK3% %AI_SOURCES_WEEK4% -o self_play.exe
if %errorlevel% == 0 (
    echo [OK] self_play.exe built
) else (
    echo [FAIL] self_play.exe build failed
)
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

