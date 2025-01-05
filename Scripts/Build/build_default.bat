:: Builds any of the default assets that need it, then wraps them.
@echo off

:: Setup
call %~dp0build_setup.bat

:: Compile all shaders
call %MINTY_PATH%Scripts\Build\compile_shaders.bat %MINTY_PATH%

:: Wrap files using the Minty build version
pushd %MINTY_PATH%Data
call %wrap% Default -v %%MintyBuild%% -b Defaults
popd