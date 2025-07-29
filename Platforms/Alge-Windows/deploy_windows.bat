@echo off
copy Debug\AlgeApp.exe .\%1.exe
xcopy ..\..\Apps\%1.Assets\Data\*.* %userprofile% /E /C /I /Y
echo %1 App Deployed for system use (assets coped to %userprofile%).
pause