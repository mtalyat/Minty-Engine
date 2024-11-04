:: Builds all of the assets for a given configuration, given through the first command line argument
@echo off

:: Get info && Build default files
call ..\..\Scripts\Build\build_default.bat

:: Target dir
set TARGET_DIR=%~dp0x64\%1\

:: Assets dir
set ASSETS_DIR=%~dp0Game

:: Copy default wrap to build
copy %MINTY_PATH%Data\Default.wrap %TARGET_DIR%
echo Wrapped "%MINTY_PATH%Data\Default.wrap" to "%TARGET_DIR%".

:: Build shaders in assets
call %MINTY_PATH%Scripts/Build/compile_shaders.bat %ASSETS_DIR%

:: Wrap files to configuration directory
call %wrap% %%ASSETS_DIR%% -v %%MintyBuild%%
move %~dp0Game.wrap %TARGET_DIR%
echo Wrapped "%ASSETS_DIR%" to "%TARGET_DIR%".