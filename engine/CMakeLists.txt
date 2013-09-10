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

### OpenGL
find_package(OpenGL REQUIRED)

## Qt
FIND_PACKAGE(Qt4 REQUIRED)
include(${QT_USE_FILE})

include_directories(${QT_INCLUDE_DIR})
set(CMAKE_AUTOMOC TRUE)

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

# target for lib
add_library(Open3DGraphicsAdventureEngineLib SHARED ${Open3DGraphicsAdventureEngineLibSrc} ${Open3DGraphicsAdventureEngineLibH} 
                             ${FragmentShader} ${VertexShader} ${GeometryShader})
TARGET_LINK_LIBRARIES(Open3DGraphicsAdventureEngineLib ${QT_LIBRARIES} ${OPENGL_LIBRARIES} ${OgreLibs} )

