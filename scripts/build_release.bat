@echo off
REM build_release.bat - build and (optionally) package release artifacts into dist/
SETLOCAL
set PROJ_ROOT=%~dp0..
cd /d "%PROJ_ROOT%"

REM Default: enable packaging
set ENABLE_PACKAGING=ON
if "%1"=="--no-packaging" set ENABLE_PACKAGING=OFF

rem Create dist dir (CMake will also create)
if not exist dist mkdir dist

rem Create build_release directory
if not exist build_release mkdir build_release
cd build_release

rem Configure
if "%ENABLE_PACKAGING%"=="ON" (
  cmake -G "Ninja" -DENABLE_PACKAGING=ON -DCMAKE_BUILD_TYPE=Release ..
) else (
  cmake -G "Ninja" -DENABLE_PACKAGING=OFF -DCMAKE_BUILD_TYPE=Release ..
)
if ERRORLEVEL 1 (
  echo CMake configuration failed
  exit /b 1
)

rem Build
ninja -j %NUMBER_OF_PROCESSORS%
if ERRORLEVEL 1 (
  echo Build failed
  exit /b 2
)

rem Package if requested
if "%ENABLE_PACKAGING%"=="ON" (
  echo Running CPack...
  cpack -C Release
  if ERRORLEVEL 1 (
    echo CPack failed
    exit /b 3
  )
  rem Rename package to include timestamp for easier tracking
  for /f "usebackq delims=" %%F in (`dir /b /o:-d "%~dp0\..\dist\*.zip"`) do (
    set "latest=%%F"
    goto :got_latest
  )
:got_latest
  if defined latest (
    for /f "tokens=1-6 delims=:-. " %%a in ("%date% %time%") do set TIMESTAMP=%%c%%a%%b-%%d%%e%%f
    set "newname=%~dp0\..\dist\%PROJECT_NAME%-%TIMESTAMP%-%PROCESSOR_ARCHITECTURE%.zip"
    move "%~dp0\..\dist\%latest%" "%newname%" >nul
    echo Packaged to: "%newname%"
  )
)

echo Build_release completed.
ENDLOCAL

