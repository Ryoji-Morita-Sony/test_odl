@echo OFF

set CMAKE_PATH="C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"

if exist build/ (
  rd /s /q build
)
md build

if exist install/ (
  rd /s /q install
)

set VCPKG_TARGET_TRIPLET=x64-windows-static-md
xcopy /I /E /Y third_party\ai-sdk-vcpkg-ports\ports third_party\vcpkg\ports

pushd third_party\vcpkg
CALL bootstrap-vcpkg.bat
vcpkg install log-settings:%VCPKG_TARGET_TRIPLET%
popd

pushd build
%CMAKE_PATH% ^
  -G "Visual Studio 17 2022" ^
  .. ^
  -DCMAKE_TOOLCHAIN_FILE=..\third_party\vcpkg\scripts\buildsystems\vcpkg.cmake ^
  -DVCPKG_TARGET_TRIPLET=%VCPKG_TARGET_TRIPLET%
popd

echo ===== Creating visual studio solution completed. =====

echo ===== Start building third party libraries. =====
call build_third_party_vs2022.bat
echo ===== End building third party libraries. =====

PAUSE
