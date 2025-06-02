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
  MLMATH_INCLUDE_DIR
  NAMES mle/mlmath.h
  PATHS $ENV{MLE_ROOT}/include)

find_library(
  MLMATH_LIBRARY_DEBUG
  NAMES mlertd
  PATHS $ENV{MLE_ROOT}/lib
  )

find_library(
  MLMATH_LIBRARY_RELEASE
  NAMES mlert
  PATHS $ENV{MLE_ROOT}/lib
  )

select_library_configurations(MLMATH)

if(MLMATH_INCLUDE_DIR AND EXISTS "${MLMATH_INCLUDE_DIR}/mle/mlmath.h")
  file(STRINGS "${MLMATH_INCLUDE_DIR}/mle/mlmath.h" _MLMATH_VERSION_DEFINE REGEX "[\t ]*#define[\t ]+MLMATH_VERSION[\t ]+\"[^\"]*\".*")
  string(REGEX REPLACE "[\t ]*#define[\t ]+MLMATH_VERSION[\t ]+\"([^\"]*)\".*" "\\1" MLMATH_VERSION "${_MLMATH_VERSION_DEFINE}")
  unset(_MLMATH_VERSION_DEFINE)
endif()

set(MLMATH_DEFINITIONS)
set(MLMATH_INCLUDE_DIRS ${MLMATH_INCLUDE_DIR})
set(MLMATH_LIBRARIES ${MLMATH_LIBRARY})

find_package_handle_standard_args(
  MLMATH
  FOUND_VAR MLMATH_FOUND
  REQUIRED_VARS MLMATH_INCLUDE_DIR MLMATH_LIBRARY
  VERSION_VAR MLMATH_VERSION
  )

if(MLMATH_FOUND AND NOT TARGET MLMath::MLMath)
  add_library(MlMath::MlMath UNKNOWN IMPORTED)
  if(MLMATH_LIBRARY_RELEASE)
    set_property(TARGET MlMath::MlMath APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
    set_target_properties(MlMath::MlMath PROPERTIES IMPORTED_LOCATION_RELEASE "${MLMATH_LIBRARY_RELEASE}")
  endif()
  if(MlMath_LIBRARY_DEBUG)
    set_property(TARGET MlMath::MlMath APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
    set_target_properties(MlMath::MlMath PROPERTIES IMPORTED_LOCATION_DEBUG "${MLMATH_LIBRARY_DEBUG}")
  endif()
  set_target_properties(MlMath::MlMath PROPERTIES INTERFACE_COMPILE_DEFINITIONS "COIN_DLL")
  set_target_properties(MlMath::MlMath PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${MLMATH_INCLUDE_DIRS}")
endif()

mark_as_advanced(MLMATH_DEFINITIONS)
mark_as_advanced(MLMATH_INCLUDE_DIRS)
mark_as_advanced(MLMATH_LIBRARIES)
