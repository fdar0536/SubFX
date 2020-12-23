rem get bat's path
echo %~dp0 > "%TEMP%\tmp.txt"
set /p CURRENT_PATH=<"%TEMP%\tmp.txt"
cd /D "%CURRENT_PATH%"

rem get python's site-packages path
python -c ^
    "from distutils.sysconfig import get_python_lib; print(get_python_lib(plat_specific=True))" > "%TEMP%\tmp.txt"
set /p PYPATH=<"%TEMP%\tmp.txt"

rem copy python module
cd lib/site-packages
for %%i in (.\*) do copy "%%i" "%PYPATH%\%%i"

cd ..
rem "rmdir /S /Q" equals to "rm -rf"
rmdir /S /Q site-packages

rem set environment variable for python
setx SUBFX_HOME "%CURRENT_PATH%" /m

pause
