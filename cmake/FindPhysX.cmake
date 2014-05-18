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

  set(PHYSX_LIBRARIES_TO_FIND

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
RepX3
RepXUpgrader3
SceneQuery
SimulationController
PhysX3

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
