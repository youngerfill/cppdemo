@echo off
pushd %~dp0
Y:\dev\mingw32make\bin\mingw32-make.exe
clapitest.exe -clapi index numsamples
echo Exit status: %ERRORLEVEL%
popd
