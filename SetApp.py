import sys
import subprocess
import os

Orientation = 'portrait'

ok = False

if len(sys.argv)==3:
    Orientation = sys.argv[2]
    # Access command-line arguments
    AppName = sys.argv[1]
    ok = True

if len(sys.argv)==2:
    AppName = sys.argv[1]
    ok = True

if not ok:
    print("Missing Arguments, At least AppName is required, Usage: SetApp <AppName> <Orientation>")
    sys.exit(1)

os.chdir("Alge-Android-Jetpack/")

subprocess.call(["collect.bat", AppName])
print("Updated Assets for android for "+ AppName + " using collect.bat")

with open('app/src/main/AndroidManifest.xml') as AndroidManifest:
    manifest = AndroidManifest.readlines()
    indexLbl = -1
    indexOri = -1
    ii = 0
    for aline in manifest:
        if ":label" in aline:
            indexLbl = ii
        if ":screenOrientation" in aline:
            indexOri = ii               
        ii = ii + 1

    if indexOri>0 and ":screenOrientation" in manifest[indexOri]:
        if '>' in manifest[indexOri]:
            manifest[indexOri] = '\t\t\tandroid:screenOrientation="'+Orientation+'">\n'
        else:
            manifest[indexOri] = '\t\t\tandroid:screenOrientation="'+Orientation+'"\n'


    if indexLbl>0 and ":label" in manifest[indexLbl]:
        if '>' in manifest[indexLbl]:
            manifest[indexLbl] = '\t\t\tandroid:label="'+AppName+'">\n'
        else:
            manifest[indexLbl] = '\t\t\tandroid:label="'+AppName+'"\n'

with open('app/src/main/AndroidManifest.xml', "w") as AndroidManifest:
    AndroidManifest.writelines(manifest)
    print("Updated app/src/main/AndroidManifest.xml for "+ AppName + " with orientation=" + Orientation)


with open('..\candidate.h') as candidate:
    fewlines = candidate.readlines()
    indexHpp = -1
    ii = 0
    for aline in fewlines:
        if ".hpp" in aline and "#include" in aline:
            indexHpp = ii
        ii = ii + 1

    if indexHpp>0:
        fewlines[indexHpp] = '#include "../Apps/'+AppName+'/'+AppName+'.hpp"\t//This is your main file\n'
        

with open('..\candidate.h', "w") as candidate:
    candidate.writelines(fewlines)
    print("Updated CANDIDATE.h for "+ AppName)

print(".done.")
