@echo off
REM clean_dist.bat - keep only N latest packages in dist/
SETLOCAL
set PROJ_ROOT=%~dp0..
cd /d "%PROJ_ROOT%"

set KEEPCOUNT=3
if not "%1"=="" set KEEPCOUNT=%1

if not exist dist (
  echo No dist directory; nothing to clean.
  exit /b 0
)

REM Use PowerShell to delete older files while keeping newest KEEPCOUNT
powershell -Command ^
  $keep = %KEEPCOUNT%; ^
  $files = Get-ChildItem -Path '%CD%\\dist' -Filter *.zip | Sort-Object LastWriteTime -Descending; ^
  $files | Select-Object -Skip $keep | ForEach-Object { Write-Output \"Removing $_.FullName\"; Remove-Item $_.FullName -Force }

echo Cleaned dist/ (kept %KEEPCOUNT% newest).
ENDLOCAL

