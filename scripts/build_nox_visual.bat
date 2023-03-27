@echo off

if not exist ..\build\ mkdir ..\build\

pushd %~dp0\..\build\
cmake .. -DNOX_BUILD_EXAMPLES=ON
popd
pause
