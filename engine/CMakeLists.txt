cmake_minimum_required(VERSION 2.8)
project(Open3DGraphicsAdventureEngine)

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

# include dirs
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${OGRE_INCLUDE_DIRS})

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

# disable caelum
# set(CaelumLib Caelum)

set(OgreCCSLib CCS)


# target for lib
add_library(Open3DGraphicsAdventureEngineLib SHARED ${Open3DGraphicsAdventureEngineLibSrc}
                                                    ${Open3DGraphicsAdventureEngineLibH}
                             ${FragmentShader} ${VertexShader} ${GeometryShader})
TARGET_LINK_LIBRARIES(Open3DGraphicsAdventureEngineLib ${QT_LIBRARIES} ${OPENGL_LIBRARIES}
                                 ${OGRE_LIBRARIES} ${OIS_LIBRARIES}  ${OgreCCSLib}
                                    ${HYDRAX_LIBRARIES}
                                # ${CaelumLib}
                                ${SKYX_LIBRARIES}
                                ${OGREAL_LIBRARIES}
)

