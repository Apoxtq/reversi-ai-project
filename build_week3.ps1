# Week 3 Build Script
Write-Host "================================================" -ForegroundColor Cyan
Write-Host "Week 3 - Minimax Engine Compilation" -ForegroundColor Cyan
Write-Host "================================================" -ForegroundColor Cyan
Write-Host ""

# Step 1: Clean build directory
Write-Host "[0/3] Cleaning build directory..." -ForegroundColor Yellow
if (Test-Path "build") {
    Remove-Item -Recurse -Force "build"
}
Write-Host "✓ Clean complete" -ForegroundColor Green
Write-Host ""

# Step 2: Configure CMake
Write-Host "[1/3] Configuring CMake..." -ForegroundColor Yellow
$configArgs = @(
    "-B", "build",
    "-G", "MinGW Makefiles",
    "-DCMAKE_BUILD_TYPE=Release"
)
$configResult = & cmake $configArgs 2>&1
Write-Host $configResult

if ($LASTEXITCODE -ne 0) {
    Write-Host "✗ ERROR: CMake configuration failed!" -ForegroundColor Red
    exit 1
}
Write-Host "✓ Configuration complete" -ForegroundColor Green
Write-Host ""

# Step 3: Build project
Write-Host "[2/3] Building project..." -ForegroundColor Yellow
$buildResult = & cmake --build build --config Release 2>&1
Write-Host $buildResult

if ($LASTEXITCODE -ne 0) {
    Write-Host "✗ ERROR: Build failed!" -ForegroundColor Red
    exit 1
}
Write-Host "✓ Build complete" -ForegroundColor Green
Write-Host ""

# Step 4: Run tests
Write-Host "[3/3] Running tests..." -ForegroundColor Yellow
Write-Host ""
Write-Host "--- Minimax Engine Test ---" -ForegroundColor Cyan

if (Test-Path "build\bin\test_minimax.exe") {
    & "build\bin\test_minimax.exe"
    $testResult = $LASTEXITCODE
    
    Write-Host ""
    if ($testResult -eq 0) {
        Write-Host "✓ All tests passed!" -ForegroundColor Green
    } else {
        Write-Host "✗ Some tests failed (exit code: $testResult)" -ForegroundColor Yellow
    }
} else {
    Write-Host "✗ test_minimax.exe not found!" -ForegroundColor Red
}

Write-Host ""
Write-Host "================================================" -ForegroundColor Cyan
Write-Host "Build Summary" -ForegroundColor Cyan
Write-Host "================================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Executables in build\bin:" -ForegroundColor White

if (Test-Path "build\bin") {
    Get-ChildItem "build\bin\*.exe" | ForEach-Object {
        $size = [math]::Round($_.Length / 1KB, 2)
        Write-Host "  ✓ $($_.Name) ($size KB)" -ForegroundColor Green
    }
} else {
    Write-Host "  (no executables found)" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "Press any key to continue..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")

