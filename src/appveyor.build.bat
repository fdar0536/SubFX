rem update vcpkg
cd /D c:\Tools\vcpkg
git pull
call bootstrap-vcpkg.bat
vcpkg integrate install

rem update vcpkg packages
vcpkg upgrade --no-dry-run

vcpkg install boost-regex[icu]:x64-windows boost-locale[icu]:x64-windows ^
    pybind11:x64-windows nlohmann-json:x64-windows cxxopts:x64-windows

rem start build project
cd /
cd projects/SubFX/src
mkdir build-windows
cd build-windows
cmake -DCMAKE_TOOLCHAIN_FILE=c:/Tools/vcpkg/scripts/buildsystems/vcpkg.cmake ^
    -G "Visual Studio 16 2019" -DPython_ROOT_DIR=C:/Python38-x64 ^
    -DCMAKE_INSTALL_PREFIX="SubFX-Release" ..
msbuild SubFX.sln /p:Configuration=Release /target:ALL_BUILD
msbuild INSTALL.vcxproj /p:Configuration=Release

cd SubFX-Release
copy "..\..\build\windows\install.bat" "install.bat"
copy "..\..\build\windows\uninstall.bat" "uninstall.bat"

cd bin
rem copy boost's dll
copy "c:\Tools\vcpkg\installed\x64-windows\bin\boost_regex-vc142-mt-x64-1_73.dll" ^
    "boost_regex-vc142-mt-x64-1_73.dll"

rem copy icu's dll
copy "c:\Tools\vcpkg\installed\x64-windows\bin\icuin67.dll" ^
    "icuin67.dll"
copy "c:\Tools\vcpkg\installed\x64-windows\bin\icuuc67.dll" ^
    "icuuc67.dll"
copy "c:\Tools\vcpkg\installed\x64-windows\bin\icudt67.dll" ^
    "icudt67.dll"

rem prepare for testing
cd ../..

rem install font for testing cases
powershell -Command "(New-Object Net.WebClient).DownloadFile('https://github.com/adobe-fonts/source-code-pro/blob/release/TTF/SourceCodePro-Regular.ttf?raw=true', 'SourceCodePro-Regular.ttf')"
reg add "HKCU\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts" /v "Source Code Pro (TrueType)" /t REG_SZ /d SourceCodePro-Regular.ttf /f

set PATH=%PATH%;%CD%\SubFX-Release\bin;
cd YutilsCpp\test\assparser\Release
copy "..\in.ass" "in.ass"
cd ../../../../
msbuild RUN_TESTS.vcxproj /p:Configuration=Release
if not errorlevel 0 (
    exit /b 1
)

7z.exe a -t7z SubFX-Release.7z SubFX-Release
