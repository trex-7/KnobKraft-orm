# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.20.21032501-MSVC_2)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(vcredist-populate NONE)



include(ExternalProject)
ExternalProject_Add(vcredist-populate
                     "UPDATE_DISCONNECTED" "False" "URL" "https://aka.ms/vs/16/release/vc_redist.x64.exe" DOWNLOAD_NO_EXTRACT YES
                    SOURCE_DIR          "C:/Users/sm/Documents/GitHub/KnobKraft-orm/build/_deps/vcredist-src"
                    BINARY_DIR          "C:/Users/sm/Documents/GitHub/KnobKraft-orm/build/_deps/vcredist-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
)


ExternalProject_Get_Property(vcredist-populate DOWNLOADED_FILE)
get_filename_component(dlFileName "${DOWNLOADED_FILE}" NAME)

ExternalProject_Add_Step(vcredist-populate copyfile
  COMMAND    "C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe" -E copy_if_different
             "<DOWNLOADED_FILE>" "C:/Users/sm/Documents/GitHub/KnobKraft-orm/build/_deps/vcredist-src"
  DEPENDEES  patch
  DEPENDERS  configure
  BYPRODUCTS "C:/Users/sm/Documents/GitHub/KnobKraft-orm/build/_deps/vcredist-src/${dlFileName}"
  COMMENT    "Copying file to SOURCE_DIR"
)

