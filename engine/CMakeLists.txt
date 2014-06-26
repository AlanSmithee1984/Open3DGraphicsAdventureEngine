cmake_minimum_required(VERSION 2.8)
project(Open3DGraphicsAdventureEngine)

set(ProjectBaseDir ${PROJECT_SOURCE_DIR}/../)




### Find out number of processor cores at the system
include(FindProcessorCount)
message("Info: used number of processor cores: " ${PROCESSOR_COUNT})
 
### Buildtype
include(HandleBuildType)

### Ogre
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    /usr/lib64/OGRE/cmake/
    /usr/lib/cmake
)


find_package(OGRE REQUIRED)
include_directories(${OGRE_INCLUDE_DIRS})

## add terrain lib
set(OGRE_LIBRARIES ${OGRE_LIBRARIES}
    /usr/lib64/libOgreTerrain.so
)


find_package(OIS REQUIRED)

## hydrax
# define HYDRAX DIRS via cmake parameter
include(FindHYDRAX)
find_package(HYDRAX REQUIRED)
include_directories(${HYDRAX_INCLUDE_DIR})

## SkyX
include(FindSkyX)
find_package(SkyX REQUIRED)
include_directories(${SKYX_INCLUDE_DIR})

## NxOgre
#include(FindNXOGRE)
#find_package(NXOGRE REQUIRED)
#include_directories(${NXOGRE_INCLUDE_DIR})

## PhysX
include(FindPhysX)
find_package(PhysX REQUIRED)
include_directories(${PHYSX_INCLUDE_DIR})
include_directories(${PHYSX_INCLUDE_DIR}/geometry)
include_directories(${PHYSX_INCLUDE_DIR}/extensions)
include_directories(${PHYSX_INCLUDE_DIR}/foundation)




#set(PHYSX_LIBRARIES
#${PHYSX_LIBRARY_LowLevel}
#${PHYSX_LIBRARY_LowLevelCloth}
#${PHYSX_LIBRARY_PhysX3}
#${PHYSX_LIBRARY_PhysX3Common}
#${PHYSX_LIBRARY_PhysX3Cooking}
#${PHYSX_LIBRARY_PhysX3Extensions}
#${PHYSX_LIBRARY_PhysXProfileSDK}
#${PHYSX_LIBRARY_PxTask}
#${PHYSX_LIBRARY_SceneQuery}
#${PHYSX_LIBRARY_SimulationController}
#)
#message("physx: " ${PHYSX_LIBRARIES})


## OgrePhysX
include(FindOgrePhysX)
find_package(OgrePhysX REQUIRED)
include_directories(${OgrePhysX_INCLUDE_DIR})

find_package(CCS REQUIRED)
include_directories(${CCS_INCLUDE_DIR})
#set(OgreCCSLib CCS)


### OpenGL
find_package(OpenGL REQUIRED)

# sound stuff
### OpenAL
find_package(OpenAL REQUIRED)
find_package(OGG REQUIRED)
find_package(VORBIS REQUIRED)

### OgreAL
find_package(OGREAL REQUIRED)
include_directories(${OGREAL_INCLUDE_DIRS})

## Qt
FIND_PACKAGE(Qt4 REQUIRED)
include(${QT_USE_FILE})
include_directories(${QT_INCLUDE_DIR})
set(CMAKE_AUTOMOC TRUE)

## TinyXML
find_package(TinyXML REQUIRED)
#include_directories(${OgrePhysX_INCLUDE_DIR})

# include dirs
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include)


# include generated header
include_directories(${PROJECT_BINARY_DIR})

file(GLOB Open3DGraphicsAdventureEngineLibSrc ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB Open3DGraphicsAdventureEngineLibH ${PROJECT_SOURCE_DIR}/src/*.h
                       ${PROJECT_SOURCE_DIR}/include/*.h)

# export header files and dirs to parent script
file(GLOB PublicOpen3DGraphicsAdventureEngineLibH ${PROJECT_SOURCE_DIR}/include/*.h)
set(PublicOpen3DGraphicsAdventureEngineLibH ${PublicOpen3DGraphicsAdventureEngineLibH} PARENT_SCOPE)
set(PublicOpen3DGraphicsAdventureEngineIncludeDir ${PROJECT_SOURCE_DIR}/include PARENT_SCOPE )


# adding shader files to project
set(ShaderPath ${ProjectBaseDir}/data/ogre/media/shader/)
file(GLOB FragmentShader ${ShaderPath}/*.frag)
file(GLOB VertexShader ${ShaderPath}/*.vert)
file(GLOB GeometryShader ${ShaderPath}/*.geom)

# material files
set(MaterialPath ${ProjectBaseDir}/data/ogre/media/materials/scripts)
file(GLOB MaterialFiles ${MaterialPath}/*.material)

# disable caelum
# set(CaelumLib Caelum)



# target for lib
add_library(Open3DGraphicsAdventureEngine SHARED ${Open3DGraphicsAdventureEngineLibSrc}
                                                    ${Open3DGraphicsAdventureEngineLibH}
                             ${FragmentShader} ${VertexShader} ${GeometryShader}
                            ${MaterialFiles}


)

TARGET_LINK_LIBRARIES(Open3DGraphicsAdventureEngine ${QT_LIBRARIES} ${OPENGL_LIBRARIES}
                                 ${OGRE_LIBRARIES} ${OIS_LIBRARIES}
                                ${CCS_LIBRARY}
                                    ${HYDRAX_LIBRARIES}
                                # ${CaelumLib}
                                ${SKYX_LIBRARIES}
                                ${OGREAL_LIBRARIES}
#                               ${NXOGRE_LIBRARY}

                                ${OgrePhysX_LIBRARY}

#                                ${PHYSX_LIBRARIES}

                                ${TINYXML_LIBRARIES}


)

