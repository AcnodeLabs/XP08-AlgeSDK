powershell.exe -ExecutionPolicy RemoteSigned -File %USERPROFILE%\beep.ps1
powershell.exe -Command "& Stop-Process -Name devenv"
start Alge_VS.sln 
exit