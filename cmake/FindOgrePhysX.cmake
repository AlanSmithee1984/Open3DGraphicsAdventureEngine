# - Locate OgrePhysX LIBRARIES
# This module defines
#  OgrePhysX_FOUND, if false, do not try to link to OgrePhysX
#  OgrePhysX_INCLUDE_DIR, where to find headers.
#  OgrePhysX_LIBRARIES, the LIBRARIES to link against
#  OgrePhysX_BINARY_REL - location of the main bullet binary (release)
#  OgrePhysX_BINARY_DBG - location of the main bullet binaries (debug)

include(FindPkgMacros)
include(PreprocessorUtils)
findpkg_begin(OgrePhysX)
set(OgrePhysX_FIND_REQUIRED 1)

# Get path, convert backslashes as ${ENV_${var}}
getenv_path(OgrePhysX_HOME)
getenv_path(OgrePhysX_DIR)
getenv_path(OgrePhysX_ROOT)
getenv_path(PROGRAMFILES)

# construct search paths from environmental hints and
# OS specific guesses
if (WIN32)
  set(OgrePhysX_PREFIX_GUESSES
    ${ENV_PROGRAMFILES}/OgrePhysX
    [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;OgrePhysX_HOME]
    [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;OgrePhysX_DIR]
    [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;OgrePhysX_ROOT]
  )
elseif (UNIX)
  set(OgrePhysX_PREFIX_GUESSES
    /opt/OgrePhysX
    /usr
    /usr/local
    $ENV{HOME}/OgrePhysX
  )
endif ()

set(OgrePhysX_PREFIX_PATH
   $ENV{OgrePhysX_HOME} $ENV{OgrePhysX_DIR} $ENV{OgrePhysX_ROOT}
   ${OgrePhysX_PREFIX_GUESSES}
)
create_search_paths(OgrePhysX)

set(OgrePhysX_RESET_VARS
  OgrePhysX_INCLUDE_DIR
  OgrePhysX_LIBRARY_REL OgrePhysX_LIBRARY_DBG)

set(OgrePhysX_PREFIX_WATCH ${OgrePhysX_PREFIX_PATH})
clear_if_changed(OgrePhysX_PREFIX_WATCH ${OgrePhysX_RESET_VARS})

# try to locate OgrePhysX via pkg-config
use_pkgconfig(OgrePhysX_PKGC "OgrePhysX")

# locate OgrePhysX include files

find_path(OgrePhysX_INCLUDE_DIR NAMES OgrePhysX.h HINTS ${OgrePhysX_INC_SEARCH_PATH} ${OgrePhysX_FRAMEWORK_INCLUDES} ${OgrePhysX_PKGC_INCLUDE_DIRS} PATH_SUFFIXES "" "OgrePhysX" "Ogrephysx" "ogrephysx")
get_debug_names(OgrePhysX_LIBRARY_NAMES)
find_library(OgrePhysX_LIBRARY_REL NAMES ${OgrePhysX_LIBRARY_NAMES} HINTS ${OgrePhysX_LIB_SEARCH_PATH} ${OgrePhysX_PKGC_LIBRARY_DIRS} ${OgrePhysX_FRAMEWORK_SEARCH_PATH} PATH_SUFFIXES "" "release" "relwithdebinfo" "minsizerel")
find_library(OgrePhysX_LIBRARY_DBG NAMES ${OgrePhysX_LIBRARY_NAMES_DBG} HINTS ${OgrePhysX_LIB_SEARCH_PATH} ${OgrePhysX_PKGC_LIBRARY_DIRS} ${OgrePhysX_FRAMEWORK_SEARCH_PATH} PATH_SUFFIXES "" "debug")
make_library_set(OgrePhysX_LIBRARY)


list(REMOVE_DUPLICATES OgrePhysX_INCLUDE_DIR)
findpkg_finish(OgrePhysX)
add_parent_dir(OgrePhysX_INCLUDE_DIRS OgrePhysX_INCLUDE_DIR)


if (NOT OgrePhysX_FOUND)
  return()
endif ()



get_filename_component(OgrePhysX_LIBRARY_DIR_REL "${OgrePhysX_LIBRARY_REL}" PATH)
get_filename_component(OgrePhysX_LIBRARY_DIR_DBG "${OgrePhysX_LIBRARY_DBG}" PATH)
set(OgrePhysX_LIBRARY_DIRS ${OgrePhysX_LIBRARY_DIR_REL} ${OgrePhysX_LIBRARY_DIR_DBG})


# find binaries
if (WIN32)
   find_file(OgrePhysX_BINARY_REL NAMES "OgrePhysX.dll" HINTS ${OgrePhysX_BIN_SEARCH_PATH} PATH_SUFFIXES "" "release" "relwithdebinfo" "minsizerel")
   find_file(OgrePhysX_BINARY_DBG NAMES "OgrePhysX_d.dll" HINTS ${OgrePhysX_BIN_SEARCH_PATH} PATH_SUFFIXES "" "debug" )
endif()
   
get_filename_component(OgrePhysX_BINARY_DIR_REL "${OgrePhysX_BINARY_REL}" PATH)
get_filename_component(OgrePhysX_BINARY_DIR_DBG "${OgrePhysX_BINARY_DBG}" PATH)
set(OgrePhysX_LIBRARY_DIRS ${OgrePhysX_BINARY_DIR_REL} ${OgrePhysX_BINARY_DIR_DBG})
mark_as_advanced(OgrePhysX_BINARY_REL OgrePhysX_BINARY_DBG OgrePhysX_BINARY_DIR_REL OgrePhysX_BINARY_DIR_DBG)


clear_if_changed(OgrePhysX_PREFIX_WATCH)
