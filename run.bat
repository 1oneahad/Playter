@echo off
cd build
cmake --build .
if %errorlevel% neq 0 exit /b %errorlevel%
Debug\Playter.exe
cd ..