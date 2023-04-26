@echo off
setlocal enabledelayedexpansion
REM Usage:
REM generate_nox_visual.bat <BUILD_TYPE> <VISUAL_STUDIO_VERSION>

call config.cmd

set BUILD_TYPE_ARG="%1"
if %BUILD_TYPE_ARG%=="Static" (
    set BUILD_TYPE=Static
) else if %BUILD_TYPE_ARG%=="static" (
    set BUILD_TYPE=Static
) else if %BUILD_TYPE_ARG%=="Shared" (
    set BUILD_TYPE=Shared
) else if %BUILD_TYPE_ARG%=="shared" (
    set BUILD_TYPE=Shared
)

set BUILD_SHARED=OFF
if %BUILD_TYPE%==Shared (
    set BUILD_SHARED=ON
)

set VISUAL_STUDIO_VERSION_ARG="%2"
if not "%2"=="" (
    set VISUAL_STUDIO_VERSION=%2
)

call check_config.cmd
if %GOTO_FAIL%==1 (
    goto :FAIL
)

if %VISUAL_STUDIO_VERSION%==2015 (
    set COMPILER=Visual Studio 14
) else if %VISUAL_STUDIO_VERSION%==2017 (
    set COMPILER=Visual Studio 15 2017
) else if %VISUAL_STUDIO_VERSION%==2019 (
    set COMPILER=Visual Studio 16 2019
) else if %VISUAL_STUDIO_VERSION%==2022 (
    set COMPILER=Visual Studio 17 2022
)
set COMPILER="%COMPILER%"

where cmake.exe
if %ERRORLEVEL% NEQ 0 (
    echo Unable to find cmake.exe
    goto :FAIL
)

set BUILD_DIR=..\..\%BUILD_DIR%
if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

pushd %~dp0\%BUILD_DIR%
cmake .. -G%COMPILER% -DBUILD_SHARED_LIBS=%BUILD_SHARED% -DNOX_INSTALL=%NOX_INSTALL% -DNOX_BUILD_EXAMPLES=%NOX_BUILD_EXAMPLES%
if %ERRORLEVEL% NEQ 0 (
    echo CMake failed
    goto :FAIL
)
popd

pause
exit /b 0

:FAIL
echo Error occured
pause
exit /b 1
