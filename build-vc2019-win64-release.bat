echo on

set PATH=%cd%\vcpkg\installed\x64-windows\bin;%PATH%
set FONTCONFIG_FILE=%cd%\windows-fonts.conf
set PANGOCAIRO_BACKEND=fontconfig
cmake -E remove_directory build || verify
cmake -E make_directory build/vc2019-win64-release
cmake -E chdir          build/vc2019-win64-release cmake -DCMAKE_TOOLCHAIN_FILE=%cd%/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_MANIFEST_MODE=Off -DCMAKE_BUILD_TYPE=Release -DTestPrefix=vc2019-win64-release/ %cd%
cmake -E chdir          build/vc2019-win64-release cmake --build . --config Release
cmake -E chdir          build/vc2019-win64-release ctest --no-compress-output -T Test -C Release || cmake -E true

