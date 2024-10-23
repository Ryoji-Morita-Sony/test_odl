@echo OFF

set CMAKE_PATH="C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"

if exist build/ (
  rd /s /q build
)
md build

if exist install/ (
  rd /s /q install
)

pushd build
%CMAKE_PATH%  -G "Visual Studio 17 2022" ..
popd

echo ===== Creating visual studio solution completed. =====

echo ===== Start building third party libraries. =====
call build_third_party_vs2022.bat
echo ===== End building third party libraries. =====

PAUSE
