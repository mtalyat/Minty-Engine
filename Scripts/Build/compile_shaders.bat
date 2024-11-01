:: Builds all of the shaders at the path given in the first command line argument.
@echo off

:: Compile all shaders
for /r %1 %%f in (*.frag *.vert) do (
    set "input_file=%%f"
    set "output_file=%%f.spv"

    call echo Compiling "%%input_file%%" to "%%output_file%%".
    call glslc "%%input_file%%" -o "%%output_file%%"

    if %errorlevel% neq 0 (
        call echo Failed to compile "%%input_file%%".
    )
)