rem get vcpkg
cd ../..
git clone https://github.com/microsoft/vcpkg.git

cd vcpkg
call bootstrap-vcpkg.bat

rem install libraries
vcpkg install ^
    boost-regex[icu]:x64-windows boost-locale[icu]:x64-windows ^
    python3[enable-shared]:x64-windows python3:x64-windows ^
    pybind11:x64-windows nlohmann-json:x64-windows ^
    cxxopts:x64-windows

cd ..
mkdir build-windows
cd build-windows
"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath > tmp.txt
set /p VCPATH=<tmp.txt
call "%VCPATH%"/VC/Auxiliary/Build/vcvars64.bat
cmake -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake ^
    -G "Visual Studio 16 2019" -DCMAKE_INSTALL_PREFIX="SubFX-Release" ..
msbuild SubFX.sln /p:Configuration=Release /target:ALL_BUILD
msbuild INSTALL.vcxproj /p:Configuration=Release
cd SubFX-Release
copy "..\..\build\windows\install.bat" "install.bat"
copy "..\..\build\windows\uninstall.bat" "uninstall.bat"

cd bin
rem copy boost's dll
copy "..\..\..\vcpkg\installed\x64-windows\bin\boost_regex-vc142-mt-x64-1_73.dll" ^
    "boost_regex-vc142-mt-x64-1_73.dll"

rem copy icu's dll
copy "..\..\..\vcpkg\installed\x64-windows\bin\icuin67.dll" ^
    "icuin67.dll"
copy "..\..\..\vcpkg\installed\x64-windows\bin\icuuc67.dll" ^
    "icuuc67.dll"
copy "..\..\..\vcpkg\installed\x64-windows\bin\icudt67.dll" ^
    "icudt67.dll"

cd ../..
7z.exe a -t7z SubFX-Release.7z SubFX-Release
pause
