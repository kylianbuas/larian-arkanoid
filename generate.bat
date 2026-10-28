@echo off
setlocal

set "SourceDir=%~dp0"
set "OutputDir=%SourceDir%build"
set "ConfigDir=%SourceDir%setup"

if not exist "%OutputDir%" (
    mkdir "%OutputDir%"
)

if not exist "%ConfigDir%" (
    mkdir "%ConfigDir%"
)

echo Configuring CMake...
cmake -G "Visual Studio 17 2022" -A x64 -Wno-dev -S "%SourceDir%." -B "%OutputDir%"

if errorlevel 1 (
    echo.
    echo CMake configuration failed.
    pause
    exit /b 1
)

if exist "%ConfigDir%" (
    echo Copying setup files to build folder...
    xcopy "%ConfigDir%\*" "%OutputDir%\" /E /I /Y /Q
)

echo.
echo CMake configuration completed successfully.

:End
pause
endlocal