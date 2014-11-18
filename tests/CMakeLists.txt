cmake_minimum_required(VERSION 2.8)
project(Tests)

set(ProjectBaseDir ${PROJECT_SOURCE_DIR}/../)

### include additional cmake scripts
set(CMAKE_MODULE_PATH
    ${ProjectBaseDir}/cmake
    ${CMAKE_MODULE_PATH}
)


### Find out number of processor cores at the system
include(FindProcessorCount)
message("Info: used number of processor cores: " ${PROCESSOR_COUNT})
 
### Buildtype
include(HandleBuildType)

### Ogre
set(CMAKE_MODULE_PATH
    /usr/lib64/OGRE/cmake/
    ${CMAKE_MODULE_PATH}
)


find_package(OGRE REQUIRED)
include_directories(${OGRE_INCLUDE_DIR})

## Qt
FIND_PACKAGE(Qt4 REQUIRED)
SET(QT_USE_QTTEST TRUE)
include(${QT_USE_FILE})
include_directories(${QT_INCLUDE_DIR})
set(CMAKE_AUTOMOC TRUE)

## PhysX
include(FindPhysX)
find_package(PhysX)


# include dirs
message(${PublicOpen3DGraphicsAdventureEngineIncludeDir})
include_directories(${PublicOpen3DGraphicsAdventureEngineIncludeDir})
include_directories(${PrivateOpen3DGraphicsAdventureEngineIncludeDir})


# include generated header
include_directories(${PROJECT_BINARY_DIR})

# point sort test
set(TestPointSortH ${PROJECT_SOURCE_DIR}/src/testpointsort.h )
set(TestPointSortCPP ${PROJECT_SOURCE_DIR}/src/testpointsort.cpp )

add_executable(TestPointSort  ${TestPointSortH} ${TestPointSortCPP} )

TARGET_LINK_LIBRARIES(TestPointSort ${QT_LIBRARIES} ${OPENGL_LIBRARIES}
Open3DGraphicsAdventureEngine
#${OGRE_LIBRARIES}
#pthread #needed for PhysX extension lib for some reason...
#-Wl,--start-group
#${PHYSX_LIBRARIES}
#-Wl,--end-group
)
add_test(TestPointSort ${EXECUTABLE_OUTPUT_PATH}/TestPointSort)


# polyhedron local information volume calc test
set(TestPolyhedronVolumeCalcH ${PROJECT_SOURCE_DIR}/src/testpolyhedronvolumecalc.h )
set(TestPolyhedronVolumeCalcCPP ${PROJECT_SOURCE_DIR}/src/testpolyhedronvolumecalc.cpp )

add_executable(TestPolyhedronVolumeCalc  ${TestPolyhedronVolumeCalcH} ${TestPolyhedronVolumeCalcCPP} )

TARGET_LINK_LIBRARIES(TestPolyhedronVolumeCalc ${QT_LIBRARIES} ${OPENGL_LIBRARIES}
Open3DGraphicsAdventureEngine
#${OGRE_LIBRARIES}
#pthread #needed for PhysX extension lib for some reason...
#-Wl,--start-group
#${PHYSX_LIBRARIES}
#-Wl,--end-group
)
add_test(TestPolyhedronVolumeCalc ${EXECUTABLE_OUTPUT_PATH}/TestPolyhedronVolumeCalc)




