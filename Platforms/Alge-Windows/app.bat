cd ..
cd ..
if exist Apps\%1 (
    rem Exists
) else (
    mkdir Apps\%1
    mkdir Apps\%1.Assets
    mkdir Apps\%1.Assets\Data
    copy Apps\Misc\Blank.hpp Apps\%1\App.hpp
)


del Platforms\candidte.h
(

echo //				▌║█║▌│║▌│║▌║▌█║Welcome !! ▌│║▌║▌│║║▌█║▌║█

echo // Tip: Bookmark this file CANDIDATE.h for specifying App to build
echo // Available AddOns:- where CORE=SDK+BOX2D+IMGUI
echo // Windows          - CORE+FMOD+NATS
echo // macOS Intel      - CORE+FMOD+GNAT
echo // macOS Silicon    - CORE+FMOD
echo // iOS,ipadOS       - CORE+FMOD
echo // Android          - CORE

echo // Edit here to point to AlgeSDK based cross platform Candidate Application 
echo // ALGEAPPNAME will be name of Target in Mac, in Windows it is defined by ProjectName  
echo #define ALGEAPPNAME "AlgeApp" //Should Match with Product .app or .exe name
echo //Only use 1 ref in candidate.h else name picking will fail # no double slashes
echo #include "../Apps/%1/%1.hpp"
echo extern class App app;
echo // The Canditate to build is referenced above:-
echo // For uncluttered workspace 'Do "Close All but this" canditdate.h' and Right Click and Open the file referenced below,
echo // Some Samples are XGui, DodgeTheCreeps, PoppingTime, Juices, Ivank_Box2D, Planets,Ivank_Box2D/ImguiTest

echo //For ease of Development Common.Assets/Data is added in the Project Build Phase [mac], so
echo //if a new app is added of new asset is added, modify and run collect.sh in app directory once after adding assets, this will also copy the Data of the App to Common.Assets/Data, in Production Use Original App.Assets folder instead of Common.Assets
)>"Platforms\candidate.h"

del xfer.bat
(
echo del /Q Platforms\Alge-Android\app\src\main\assets\*.*
echo copy /Y Apps\%1.Assets\Data\*.* Platforms\Alge-Android\app\src\main\assets
echo copy /Y Apps\%1.Assets\Data\*.* Apps\COMMON.Assets\Data
)>"xfer.bat"

xfer.bat