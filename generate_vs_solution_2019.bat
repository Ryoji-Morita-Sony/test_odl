@echo OFF

if exist build/ (
  rd /s /q build
)
md build

if exist install/ (
  rd /s /q install
)

pushd build
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 16 2019" ..
popd

PAUSE
