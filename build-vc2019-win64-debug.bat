echo on

set PATH=%cd%\vcpkg\installed\x64-windows\bin;%PATH%
set FONTCONFIG_FILE=%cd%\windows-fonts.conf
set PANGOCAIRO_BACKEND=fontconfig
cmake -E remove_directory build || verify
cmake -E make_directory build/vc2019-win64-debug
cmake -E chdir          build/vc2019-win64-debug cmake -DCMAKE_TOOLCHAIN_FILE=%cd%/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_MANIFEST_MODE=Off -DCMAKE_BUILD_TYPE=Debug -DTestPrefix=vc2019-win64-debug/ %cd%
cmake -E chdir          build/vc2019-win64-debug cmake --build . --config Debug
cmake -E chdir          build/vc2019-win64-debug ctest --no-compress-output -T Test -C Debug || cmake -E true

