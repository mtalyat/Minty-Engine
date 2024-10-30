@echo off

:: Define the configuration file path relative to this script's location
set FILE=%~dp0minty.txt

:: Function to parse config file and set variables
:parse_config
for /f "tokens=1* delims=: " %%A in (%FILE%) do (
    set "key=%%A"
    set "value=%%B"
    :: Dynamically set variable with key name
    call set "%%key%%=%%value%%"
    call echo %%key%%=%%value%%
)

endlocal