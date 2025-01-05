Set objShell = CreateObject("WScript.Shell")

' Get the argument passed from the batch file
arg = WScript.Arguments.Item(0)

' Get the path to the VBScript
scriptPath = CreateObject("Scripting.FileSystemObject").GetParentFolderName(WScript.ScriptFullName)

' Build the command string with the full path to the batch file and log file
command = "cmd /c call """ & scriptPath & "\setup_build_config.bat"" " & arg & " >> """ & scriptPath & "\setup.log"" 2>&1"

' Execute the command silently (0 = hidden window, True = wait for completion)
objShell.Run command, 0, True