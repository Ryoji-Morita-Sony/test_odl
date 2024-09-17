@echo OFF

if exist build/ (
  rd /s /q build
)
md build

if exist install/ (
  rd /s /q install
)

pushd build
"C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 17 2022" ..
popd

PAUSE
