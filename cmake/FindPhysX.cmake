# - Locate HYDRAX LIBRARIES
# This module defines
#  PHYSX_FOUND, if false, do not try to link to HYDRAX
#  PHYSX_INCLUDE_DIR, where to find headers.
#  PHYSX_LIBRARIES, the LIBRARIES to link against
#  PHYSX_LIBRARY - location of the main bullet binary



# physics (physx)
#if(USE_PHYSX)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(PHYSX_LIB_SUFFIX "CHECKED")
  endif()
  set( PHYSX_ROOT "/usr/local" CACHE PATH "physx root" )
  set( LIB_SUFFIX 64 )
  IF(NOT EXISTS ${PHYSX_ROOT})
    message(FATAL_ERROR "physx root directory does not exist: ${PHYSX_ROOT}")
  endif ()

  FIND_PATH(
    PHYSX_INCLUDE_DIR PxPhysicsAPI.h
    PATHS "/usr/local" ${PHYSX_ROOT}
    PATH_SUFFIXES "" "Include"
    DOC "physx include directory")
  set (INCLUDES ${INCLUDES}
    ${PHYSX_INCLUDE_DIR}
  )


#set(PHYSX_RESET_VARS
#  PHYSX_INCLUDE_DIR
#  PHYSX_LIBRARY_REL
#  PHYSX_LIBRARY_DBG)

#set(PHYSX_LIBRARY_NAMES "PhysX")

#FIND_LIBRARY(
#      PHYSX_LIBRARY
#      NAMES ${PHYSX_LIBRARY_NAMES}
#      HINTS ${PHYSX_INCLUDE_DIR}/..
#      PATH_SUFFIXES "lib${LIB_SUFFIX}" "Lib/linux${LIB_SUFFIX}"
#)

#set (PHYSX_LIBRARIES ${PHYSX_LIBRARY} )


  set(PHYSX_LIBRARIES_TO_FIND

##-lpthread \
##-lrt \
##PhysX3
##SimulationController
##LowLevel
##PhysXProfileSDK
##PhysX3Extensions
###Foundation
##SceneQuery
##PhysX3Common
##PhysX3CharacterKinematic
##PhysX3Vehicle
##PhysX3Cooking
###PhysX3MetaData
###GeomUtils
##PvdRuntime
##RepX3
##PxTask



PhysX3
SimulationController
SceneQuery
LowLevel
LowLevelCloth
PhysX3Common
PhysX3Cooking
PhysX3Extensions
PhysXProfileSDK
PxTask
PhysXVisualDebuggerSDK
PvdRuntime
PhysX3CharacterKinematic
PhysX3Vehicle
#RepX3
#RepXUpgrader3
PhysX3Extensions


# from VS SLN
#Foundation
#PhysX3Common
#PhysX3CharacterKinematic
#PhysX3Cooking
#PhysX3
#LowLevel
##GeomUtils
#SceneQuery
#SimulationController
#PxTask
#PhysX3Extensions


)


foreach(LIB_TO_FIND ${PHYSX_LIBRARIES_TO_FIND})
    FIND_LIBRARY(
      PHYSX_LIBRARY_${LIB_TO_FIND}
      NAMES "${LIB_TO_FIND}${PHYSX_LIB_SUFFIX}"
      HINTS ${PHYSX_INCLUDE_DIR}/..
      PATH_SUFFIXES "lib${LIB_SUFFIX}" "Lib/linux${LIB_SUFFIX}")
    set (PHYSX_LIBRARIES ${PHYSX_LIBRARIES}
      ${PHYSX_LIBRARY_${LIB_TO_FIND}}
    )
  endforeach()


#endif(USE_PHYSX)
