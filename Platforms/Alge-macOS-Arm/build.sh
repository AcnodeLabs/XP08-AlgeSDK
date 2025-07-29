sudo xcode-select -s /Applications/Xcode.app/Contents/Developer
#xcodebuild -list -project Alge-macOS-Arm.xcodeproj
xcodebuild -scheme AlgeApp build -configuration Debug clean build CONFIGURATION_BUILD_DIR=../../../../Xcode_output
cp -f ../../../../Xcode_output/libfmod* /usr/local/lib
#open ../../../../Xcode_output/AlgeApp.app
