@echo OFF

set CMAKE_PATH="C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"

set PROJECT_DIR=%~dp0third_party\oscpack\

set BUILD_DIR=%PROJECT_DIR%build
if exist "%BUILD_DIR%" (
    echo Removing existing build directory...
    rmdir /S /Q "%BUILD_DIR%"
)

mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"

%CMAKE_PATH%  -G "Visual Studio 17 2022" -A x64 ..

%CMAKE_PATH% --build . --target oscpack --config Debug
%CMAKE_PATH% --build . --target oscpack --config Release

echo Build complete.
pause
