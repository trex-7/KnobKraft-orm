# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.20.21032501-MSVC_2)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(icu-populate NONE)



include(ExternalProject)
ExternalProject_Add(icu-populate
                     "UPDATE_DISCONNECTED" "False" "URL" "https://github.com/unicode-org/icu/releases/download/release-74-2/icu4c-74_2-Win64-MSVC2019.zip"
                    SOURCE_DIR          "C:/Users/sm/Documents/GitHub/KnobKraft-orm/build/_deps/icu-src"
                    BINARY_DIR          "C:/Users/sm/Documents/GitHub/KnobKraft-orm/build/_deps/icu-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
)


