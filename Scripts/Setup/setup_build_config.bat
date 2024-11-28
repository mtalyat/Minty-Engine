@echo off
echo ----- %SOLUTION_NAME% (%1) -----
%MSBUILD_PATH% %SOLUTION_PATH% /p:Configuration=%1 /p:Platform=x64
echo ---------------------------