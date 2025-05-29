# COPYRIGHT_BEGIN
#
# The MIT License (MIT)
#
# Copyright (c) 2025 Wizzer Works
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
#  For information concerning this header file, contact Mark S. Millard,
#  of Wizzer Works at msm@wizzerworks.com.
#
#  More information concerning Wizzer Works may be found at
#
#      http://www.wizzerworks.com
#
# COPYRIGHT_END

# Stop if cmake version is below 3.0
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# Project name and enable C++ support
project(mlert VERSION 1.0.0 LANGUAGES CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify where to find CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Fail early if one of the required packages cannot be found
include(FindGTK2)
find_package(GTK2 REQUIRED gtk)
find_package(PkgConfig REQUIRED)

pkg_check_modules (
  GTK_GL2    # variable will be used by cmake
  REQUIRED   # is required mandatory
  gtkgl-2.0  # name of package to be found
)

# Check for Magic Lantern environment variables
if (DEFINED ENV{MLE_ROOT})
  message(STATUS "MLE_ROOT=" $ENV{MLE_ROOT})
else()
  message(STATUS "MLE_ROOT NOT SET")
  set(ENV{MLE_ROOT} "/opt/MagicLantern")
  message(STATUS "Using " $ENV{MLE_ROOT})
endif()
if (DEFINED ENV{MLE_WORKPRINTS})
  message(STATUS "MLE_WORKPRINTS=" $ENV{MLE_WORKPRINTS})
else()
  message(STATUS "MLE_WORKPRINTS NOT SET")
  set(ENV{MLE_ROOT} "/opt/MagicLantern/include/workprints")
  message(STATUS "Using " $ENV{MLE_WORKPRINTS})
endif()

# Specify the library
message(STATUS "Magic Lantern Variables:")
message(STATUS "    MLE_ROOT=" $ENV{MLE_ROOT})
message(STATUS "    MLE_WORKPRINTS=" $ENV{MLE_WORKPRINTS})

if(GTK2_FOUND)

  # Specify the include directories for the header files
  include_directories(
    ../../../common/src/foundation
    ../../../common/src/input
    ../../../common/include
    ../../../linux/include
    $ENV{MLE_ROOT}/include
    ${GTK2_INCLUDE_DIRS})

  # Specify the compiler macro definitions
  add_compile_definitions(
    MLE_NOT_UTIL_DLL
    MLE_NOT_MATH_DLL
    MLE_NOT_DWP_DLL
    MLE_NOT_RUNTIME_DLL
    MLE_INTERNAL
    MLE_DIGITAL_PLAYPRINT
    ML_MATH_DEBUG=0
    ML_FIXED_POINT=0
    MLE_GTK)

  # Specify the shared library
  add_library(
    mlertShared SHARED
    ../../../common/src/foundation/3danim.cxx
    ../../../common/src/foundation/3dchar.cxx
    ../../../common/src/foundation/MleActor.cxx
    ../../../common/src/foundation/MleActorGC.cxx
    ../../../common/src/foundation/MleDirector.cxx
    ../../../common/src/foundation/MleDso.cxx
    ../../../common/src/foundation/MleEvent.cxx
    ../../../common/src/foundation/MleEventDispatcher.cxx
    ../../../common/src/foundation/MleGroup.cxx
    ../../../common/src/foundation/MleLoad.cxx
    ../../../common/src/foundation/MleMediaRef.cxx
    ../../../common/src/foundation/MleMonitor.cxx
    ../../../common/src/foundation/MleNotifier.cxx
    ../../../common/src/foundation/MleObject.cxx
    ../../../common/src/foundation/MlePq.cxx
    ../../../common/src/foundation/MlePtrArray.cxx
    ../../../common/src/foundation/MlePtrDict.cxx
    ../../../common/src/foundation/MleRole.cxx
    ../../../common/src/foundation/MleScene.cxx
    ../../../common/src/foundation/MleScheduler.cxx
    ../../../common/src/foundation/MleSet.cxx
    ../../../common/src/foundation/MleStage.cxx
    ../../../common/src/input/MleKeyboardPolled.cxx)

  # Specify the static library
  add_library(
    mlertStatic STATIC
    ../../../common/src/foundation/3danim.cxx
    ../../../common/src/foundation/3dchar.cxx
    ../../../common/src/foundation/MleActor.cxx
    ../../../common/src/foundation/MleActorGC.cxx
    ../../../common/src/foundation/MleDirector.cxx
    ../../../common/src/foundation/MleDso.cxx
    ../../../common/src/foundation/MleEvent.cxx
    ../../../common/src/foundation/MleEventDispatcher.cxx
    ../../../common/src/foundation/MleGroup.cxx
    ../../../common/src/foundation/MleLoad.cxx
    ../../../common/src/foundation/MleMediaRef.cxx
    ../../../common/src/foundation/MleMonitor.cxx
    ../../../common/src/foundation/MleNotifier.cxx
    ../../../common/src/foundation/MleObject.cxx
    ../../../common/src/foundation/MlePq.cxx
    ../../../common/src/foundation/MlePtrArray.cxx
    ../../../common/src/foundation/MlePtrDict.cxx
    ../../../common/src/foundation/MleRole.cxx
    ../../../common/src/foundation/MleScene.cxx
    ../../../common/src/foundation/MleScheduler.cxx
    ../../../common/src/foundation/MleSet.cxx
    ../../../common/src/foundation/MleStage.cxx
    ../../../common/src/input/MleKeyboardPolled.cxx)

  # Specify the shared library properties
  set_target_properties(mlertShared PROPERTIES
    OUTPUT_NAME mlert
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR})

  target_compile_options(mlertShared
    PRIVATE
      $<$<CONFIG:Debug>:-O0>
      $<$<CONFIG:Release>:>)

  target_compile_definitions(mlertShared
    PRIVATE
      $<$<CONFIG:Debug>: MLE_DEBUG>
      $<$<CONFIG:Release>:>)

  # Specify the static library properties
  set_target_properties(mlertStatic PROPERTIES
    OUTPUT_NAME mlert
    VERSION ${PROJECT_VERSION})

  target_compile_options(mlertStatic
    PRIVATE
      $<$<CONFIG:Debug>:-O0>
      $<$<CONFIG:Release>:>)

  target_compile_definitions(mlertStatic
    PRIVATE
      $<$<CONFIG:Debug>: MLE_DEBUG>
      $<$<CONFIG:Release>:>)

  # Install the libraries
  install(
    TARGETS
      mlertShared
      mlertStatic
    DESTINATION
      lib/mle/runtime
   )

  # Install the header files
  install(
    FILES
      ../../../common/src/foundation/mle/3danim.h
      ../../../common/src/foundation/mle/3dchar.h
      ../../../common/src/foundation/mle/MleActorClass.h
      ../../../common/src/foundation/mle/MleActorGC.h
      ../../../common/src/foundation/mle/MleActor.h
      ../../../common/src/foundation/mle/MleDirector.h
      ../../../common/src/foundation/mle/MleDso.h
      ../../../common/src/foundation/mle/MleEvent.h
      ../../../common/src/foundation/mle/MleEventDispatcher.h
      ../../../common/src/foundation/mle/MleGroupClass.h
      ../../../common/src/foundation/mle/MleGroup.h
      ../../../common/src/foundation/mle/MleLoad.h
      ../../../common/src/foundation/mle/MleMediaRefClass.h
      ../../../common/src/foundation/mle/MleMediaRefConverter.h
      ../../../common/src/foundation/mle/MleMediaRef.h
      ../../../common/src/foundation/mle/MleMonitor.h
      ../../../common/src/foundation/mle/MleNotifier.h
      ../../../common/src/foundation/mle/MleOpcode.h
      ../../../common/src/foundation/mle/MleObject.h
      ../../../common/src/foundation/mle/MlePq.h
      ../../../common/src/foundation/mle/MleProperty.h
      ../../../common/src/foundation/mle/MlePropDataset.h
      ../../../common/src/foundation/mle/MlePtrArray.h
      ../../../common/src/foundation/mle/MlePtrContainer.h
      ../../../common/src/foundation/mle/MlePtrDict.h
      ../../../common/src/foundation/mle/MleRoleClass.h
      ../../../common/src/foundation/mle/MleRole.h
      ../../../common/src/foundation/mle/MleRuntime.h
      ../../../common/src/foundation/mle/MleSceneClass.h
      ../../../common/src/foundation/mle/MleScene.h
      ../../../common/src/foundation/mle/MleScheduler.h
      ../../../common/src/foundation/mle/MleSetClass.h
      ../../../common/src/foundation/mle/MleSet.h
      ../../../common/src/foundation/mle/MleStageClass.h
      ../../../common/src/foundation/mle/MleStageFuncs.h
      ../../../common/src/foundation/mle/MleStage.h
      ../../../common/src/foundation/mle/MleTables.h
      ../../../common/src/foundation/mle/MlePlatformData.h
      ../../../common/src/input/mle/MleKeyboardEvent.h
      ../../../common/src/input/mle/MleKeyboardPolled.h
      ../../../common/src/input/mle/MleKeyMap.h
      ../../../common/src/input/mle/MleMouseEvent.h
      ../../../common/src/input/mle/MleMousePolled.h
      ../../../linux/include/mle/mlPlatformData.h
    DESTINATION
      include/mle
  )

endif()
