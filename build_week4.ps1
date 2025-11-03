# PowerShell script to build Week 4 programs
# COMP390 Honours Year Project - Week 4

Write-Host "╔════════════════════════════════════════╗" -ForegroundColor Cyan
Write-Host "║  Building Week 4 Programs              ║" -ForegroundColor Cyan
Write-Host "║  COMP390 - Advanced Evaluation         ║" -ForegroundColor Cyan
Write-Host "╚════════════════════════════════════════╝" -ForegroundColor Cyan

$ErrorActionPreference = "Continue"
$CXX = "g++"
$CXXFLAGS = "-std=c++20 -O3 -Wall -Wextra -march=native"
$INCLUDES = "-Isrc"

# Source files
$CORE_SOURCES = "src/core/Board.cpp"
$AI_SOURCES_WEEK3 = "src/ai/Evaluator.cpp src/ai/MinimaxEngine.cpp"
$AI_SOURCES_WEEK4 = "src/ai/StabilityAnalyzer.cpp src/ai/Evaluator_Week4.cpp"

Write-Host "`n[1/3] Building test_stability..." -ForegroundColor Yellow
$cmd = "$CXX $CXXFLAGS $INCLUDES tests/test_stability.cpp $CORE_SOURCES $AI_SOURCES_WEEK4 -o test_stability.exe"
Write-Host $cmd -ForegroundColor Gray
Invoke-Expression $cmd
if ($LASTEXITCODE -eq 0) {
    Write-Host "✓ test_stability.exe built successfully" -ForegroundColor Green
} else {
    Write-Host "✗ Failed to build test_stability.exe" -ForegroundColor Red
}

Write-Host "`n[2/3] Building self_play..." -ForegroundColor Yellow
$cmd = "$CXX $CXXFLAGS $INCLUDES src/research/self_play.cpp $CORE_SOURCES $AI_SOURCES_WEEK3 $AI_SOURCES_WEEK4 src/ai/MinimaxEngine.cpp -o self_play.exe"
Write-Host $cmd -ForegroundColor Gray
Invoke-Expression $cmd
if ($LASTEXITCODE -eq 0) {
    Write-Host "✓ self_play.exe built successfully" -ForegroundColor Green
} else {
    Write-Host "✗ Failed to build self_play.exe" -ForegroundColor Red
}

Write-Host "`n[3/3] Building bench_week4 (enhanced evaluator benchmark)..." -ForegroundColor Yellow
# Note: We'll create bench_week4.cpp next
# For now, skip if it doesn't exist
if (Test-Path "src/bench_week4.cpp") {
    $cmd = "$CXX $CXXFLAGS $INCLUDES src/bench_week4.cpp $CORE_SOURCES $AI_SOURCES_WEEK4 src/ai/MinimaxEngine.cpp -o bench_week4.exe"
    Write-Host $cmd -ForegroundColor Gray
    Invoke-Expression $cmd
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✓ bench_week4.exe built successfully" -ForegroundColor Green
    } else {
        Write-Host "✗ Failed to build bench_week4.exe" -ForegroundColor Red
    }
} else {
    Write-Host "⊘ bench_week4.cpp not found (will create later)" -ForegroundColor Gray
}

Write-Host "`n╔════════════════════════════════════════╗" -ForegroundColor Cyan
Write-Host "║  Build Summary                         ║" -ForegroundColor Cyan
Write-Host "╚════════════════════════════════════════╝" -ForegroundColor Cyan

Write-Host "`nWeek 4 Programs:" -ForegroundColor Cyan
if (Test-Path "test_stability.exe") {
    Write-Host "  ✓ test_stability.exe" -ForegroundColor Green
} else {
    Write-Host "  ✗ test_stability.exe (build failed)" -ForegroundColor Red
}

if (Test-Path "self_play.exe") {
    Write-Host "  ✓ self_play.exe" -ForegroundColor Green
} else {
    Write-Host "  ✗ self_play.exe (build failed)" -ForegroundColor Red
}

if (Test-Path "bench_week4.exe") {
    Write-Host "  ✓ bench_week4.exe" -ForegroundColor Green
}

Write-Host "`nTo run tests:" -ForegroundColor Yellow
Write-Host "  .\test_stability.exe         # Test stability analysis"
Write-Host "  .\self_play.exe 20           # Run 20 self-play games"

Write-Host "`nNote: Make sure you have GCC 13+ with C++20 support" -ForegroundColor Gray

