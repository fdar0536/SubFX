rem delete python module
rem get python's site-packages path
python -c ^
    "from distutils.sysconfig import get_python_lib; print(get_python_lib(plat_specific=True))" > "%TEMP%\tmp.txt"
set /p PYPATH=<"%TEMP%\tmp.txt"

rem remove python module
rem "del /F" equals to "rm -f"
del /F /Q "%PYPATH%\SubFX_YutilsPy.pyd"
del /F /Q "%PYPATH%\SubFX_YutilsPy.lib"
del /F /Q "%PYPATH%\SubFX_AssPy.py"

rem remove environment variable
REG delete "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /F /V SUBFX_HOME

pause
