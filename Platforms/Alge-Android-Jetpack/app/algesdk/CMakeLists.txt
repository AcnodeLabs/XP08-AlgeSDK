
# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project("Alge-Android-Jetpack")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

include_directories (
        ../../../../../imgui
        ../../../../../Box2D/Box2D
)

add_library( # Sets the name of the library.
        algesdk

        # Sets the library as a shared library.
        SHARED
        # Provides a relative path to your source file(s).
        ../../../../../imgui/imgui.cpp   
        ../../../../../imgui/imgui_demo.cpp      
        ../../../../../imgui/imgui_draw.cpp      
        ../../../../../imgui/imgui_tables.cpp
        ../../../../../imgui/imgui_widgets.cpp
        ../../../../../imgui/imgui_widgets.cpp
        ../../../../Platforms/Alge-Android-Jetpack/imgui_impl_android.cpp
        ../../../../Platforms/Alge-Android-Jetpack/imgui_impl_opengl2.cpp
        ../../../../SDKSRC/Base/jpgd.cpp
        ../../../../SDKSRC/Base/camera.cpp
        device-app.cpp
        app-android.c
        importgl.c
        Box2D.cpp
        )


# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

##find_library( # Sets the name of the path variable.
##              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
##             log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        algesdk
        GLESv1_CM
        dl
        android
        log
        #input
)
                       # Links the target library to the log library
                       # included in the NDK.
#                       ${log-lib} )
