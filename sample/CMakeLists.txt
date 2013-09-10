cmake_minimum_required(VERSION 2.8)
project(SampleProject)

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
#find_package(OpenGL REQUIRED)

## Qt
FIND_PACKAGE(Qt4 REQUIRED)
include(${QT_USE_FILE})
include_directories(${QT_INCLUDE_DIR})
set(CMAKE_AUTOMOC TRUE)

# include dirs
message(${PublicOpen3DGraphicsAdventureEngineIncludeDir})
include_directories(${PublicOpen3DGraphicsAdventureEngineIncludeDir})


# include generated header
include_directories(${PROJECT_BINARY_DIR})

file(GLOB SampleProjectSrc ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB SampleProjectH ${PROJECT_SOURCE_DIR}/*.h)

# target for sample exec
add_executable(  SampleProject ${SampleProjectSrc} ${SampleProjectH}  )
TARGET_LINK_LIBRARIES(SampleProject ${QT_LIBRARIES} ${OPENGL_LIBRARIES} ${OgreLibs} Open3DGraphicsAdventureEngineLib)

add_custom_command(TARGET SampleProject
    POST_BUILD

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ProjectBaseDir}/data/ogre
         ${PROJECT_BINARY_DIR}

    VERBATIM
)



