:: Builds a project (all configurations), at the same time.
@echo off
set MSBUILD_PATH="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
set SOLUTION_PATH=%1
for %%F in ("%SOLUTION_PATH%") do set SOLUTION_NAME=%%~nxF

echo ----- %SOLUTION_NAME% -----

:: Build both configs.
call :run_build Debug
call :run_build Release

echo -------------------------

exit /b

:run_build
echo Building %1.
title %SOLUTION_NAME% (%1)
wscript.exe //nologo "%~dp0setup_build_silent.vbs" "%1"
@REM start /wait cmd /c "call %~dp0setup_build_config.bat Debug >> %~dp0debug_build.log 2>&1"
echo Completed %1.