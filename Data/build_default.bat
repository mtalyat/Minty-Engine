@echo off

:: Get shader info
pushd %~dp0\..\Info
call parse_minty.bat
popd

::      Set path to compiler
set GLSLC="glslc"

::      Set path to target directory
set TARGET_DIR="%~dp0\default"

::      Compile all shaders
for /r %TARGET_DIR% %%f in (*.frag *.vert) do (
    set "input_file=%%f"
    set "output_file=%%~dpnf.spv"

    call echo Compiling "%%input_file%%" to "%%output_file%%".
    call %GLSLC% "%%input_file%%" -o "%%output_file%%"

    if %errorlevel% neq 0 (
        call echo Failed to compile "%%input_file%%".
    )
)

:: Wrap files
call wrap "%~dp0\default" -v %%MintyBuild%% -b Defaults