@echo off
python3 SetApp.py %1 %2
cd ..\Apps\
xcopy %1.Assets\Data\ ..\Platforms\Alge-macOS-Arm\Data\ /e /y
xcopy %1.Assets\Data\ ..\Platforms\Alge-iOS-Arm\Data\ /e /y
cd ..
cd Tools\scripts
close_and_open.bat %1
echo %1 App is now active (Assets Dir = %1.Assets/Data).
cd %ALGEWINDOWS%
cd ..
