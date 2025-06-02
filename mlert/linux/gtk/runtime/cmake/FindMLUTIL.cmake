include(FindPackageHandleStandardArgs)
include(SelectLibraryConfigurations)

if (DEFINED ENV{MLE_ROOT})
  message(STATUS "MLE_ROOT=" $ENV{MLE_ROOT})
else()
  message(STATUS "MLE_ROOT NOT SET")
  set(ENV{MLE_ROOT} "/opt/MagicLantern")
  message(STATUS "Using " $ENV{MLE_ROOT})
endif()

find_path(
  MLUTIL_INCLUDE_DIR
  NAMES mle/mle.h
  PATHS $ENV{MLE_ROOT}/include)

find_library(
  MLUTIL_LIBRARY_DEBUG
  NAMES mlutild
  PATHS $ENV{MLE_ROOT}/lib
  PATH_SUFFIXES mle
  )

find_library(
  MLUTIL_LIBRARY_RELEASE
  NAMES mlutil
  PATHS $ENV{MLE_ROOT}/lib
  PATH_SUFFIXES mle
  )

select_library_configurations(MLUTIL)

if(MLMath_INCLUDE_DIR AND EXISTS "${MLUTIL_INCLUDE_DIR}/mle/mlutil.h")
  file(STRINGS "${MLUTIL_INCLUDE_DIR}/mle/mlutil.h" _MLUTIL_VERSION_DEFINE REGEX "[\t ]*#define[\t ]+MLUTIL_VERSION[\t ]+\"[^\"]*\".*")
  string(REGEX REPLACE "[\t ]*#define[\t ]+MLUTIL_VERSION[\t ]+\"([^\"]*)\".*" "\\1" MLUTIL_VERSION "${_MLUTIL_VERSION_DEFINE}")
  unset(_MLUTIL_VERSION_DEFINE)
endif()

set(MLUTIL_DEFINITIONS)
set(MLUTIL_INCLUDE_DIRS ${MLUTIL_INCLUDE_DIR})
set(MLUTIL_LIBRARIES ${MLUTIL_LIBRARY})

find_package_handle_standard_args(
  MLUTIL
  FOUND_VAR MLUTIL_FOUND
  REQUIRED_VARS MLUTIL_INCLUDE_DIR MLUTIL_LIBRARY
  VERSION_VAR MLUTIL_VERSION
  )

if(MLUTIL_FOUND AND NOT TARGET MlUtil::MlUtil)
  add_library(MlUtil::MlUtil UNKNOWN IMPORTED)
  if(MLUTIL_LIBRARY_RELEASE)
    set_property(TARGET MlUtil::MlUtil APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
    set_target_properties(MlUtil::MlUtil PROPERTIES IMPORTED_LOCATION_RELEASE "${MLUTIL_LIBRARY_RELEASE}")
  endif()
  if(MlUtil_LIBRARY_DEBUG)
    set_property(TARGET MlUtil::MlUtil APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
    set_target_properties(MlUtil::MlUtil PROPERTIES IMPORTED_LOCATION_DEBUG "${MLUTIL_LIBRARY_DEBUG}")
  endif()
  set_target_properties(MlUtil::MlUtil PROPERTIES INTERFACE_COMPILE_DEFINITIONS "COIN_DLL")
  set_target_properties(MlUtil::MlUtil PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${MLUTIL_INCLUDE_DIRS}")
endif()

mark_as_advanced(MLUTIL_DEFINITIONS)
mark_as_advanced(MLUTIL_INCLUDE_DIRS)
mark_as_advanced(MLUTIL_LIBRARIES)
