:: Builds all of the assets for a given configuration, given through the first command line argument
@echo off

:: Get info && Build default files
call ..\..\Scripts\Build\build_default.bat
if %errorlevel% neq 0 exit /b %errorlevel%

:: Target dir
set TARGET_DIR=%~dp0x64\%1\

:: Assets dir
set ASSETS_DIR=%~dp0Game

:: Copy default wrap to build
copy %MINTY_PATH%Data\Default.wrap %TARGET_DIR%
if %errorlevel% neq 0 exit /b %errorlevel%
echo Wrapped "%MINTY_PATH%Data\Default.wrap" to "%TARGET_DIR%".

:: Build shaders in assets
call %MINTY_PATH%Scripts/Build/compile_shaders.bat %ASSETS_DIR%
if %errorlevel% neq 0 exit /b %errorlevel%

:: Wrap files to configuration directory
call %wrap% %%ASSETS_DIR%% -v %%MintyBuild%%
if %errorlevel% neq 0 exit /b %errorlevel%
move %~dp0Game.wrap %TARGET_DIR%
if %errorlevel% neq 0 exit /b %errorlevel%
echo Wrapped "%ASSETS_DIR%" to "%TARGET_DIR%".