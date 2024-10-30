:: Expects $1 to be Configuration (Debug/Runtime)

@echo off

:: Get info && Build default files
pushd %~dp0..\..\Data\
call build_default.bat
popd

:: Target dir
set TARGET_DIR=%~dp0x64\%1\
echo %TARGET_DIR%

:: Copy to build
copy %~dp0..\..\Data\default.wrap %TARGET_DIR%

:: Build shaders in assets
::      Set path to compiler
set GLSLC="glslc"

::      Path to target directory

::      Compile all shaders
for /r "%TARGET_DIR%game" %%f in (*.glsl *.frag *.vert) do (
    set "input_file=%%f"
    set "output_file=%%~dpnf.spv"

    call echo Compiling "%%input_file%%" to "%%output_file%%".
    call %GLSLC% "%%input_file%%" -o "%%output_file%%"

    if %errorlevel% neq 0 (
        call echo Failed to compile "%%input_file%%".
    )
)

:: Wrap files
call wrap %%TARGET_DIR%%game -v %%MintyBuild%%
popd