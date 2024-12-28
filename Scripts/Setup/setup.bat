:: Run this script to perform all of the initial setup for a first time use on a machine.
@echo off

:: Clear logs before running commands
echo %date% %time% > "%~dp0setup.log"

echo Update user files to run in the target directories.
set DEFAULT_USER_PATH=%~dp0default_user.xml
copy %DEFAULT_USER_PATH% %~dp0..\..\Assembly\Source\MintyEngine.vcxproj.user
copy %DEFAULT_USER_PATH% %~dp0..\..\Runtime\Source\MintyRuntime.vcxproj.user
copy %DEFAULT_USER_PATH% %~dp0..\..\Runtime\Test\MintyRuntimeTest.vcxproj.user
copy %DEFAULT_USER_PATH% %~dp0..\..\Editor\Source\MintyEditor.vcxproj.user

echo Creating environment variable.
setx MINTY_PATH %~dp0..\..\ /m

echo Building projects.
set SETUP_BUILD_PATH=%~dp0\setup_build.bat
call 
call %SETUP_BUILD_PATH% %~dp0..\..\Assembly\Source\MintyEngine.sln
call %SETUP_BUILD_PATH% %~dp0..\..\Runtime\Source\MintyRuntime.sln
call %SETUP_BUILD_PATH% %~dp0..\..\Runtime\Test\MintyRuntimeTest.sln
call %SETUP_BUILD_PATH% %~dp0..\..\Editor\Source\MintyEditor.sln