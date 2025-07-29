import os
cmd = 'copy /Y "..\..\Apps\Misc.Assets\Data\*.*" "..\..\Platforms\Alge-Android-Jetpack\app\src\main\assets"'
os.system(cmd)
print('assest copied to ..Android-Jetpack')