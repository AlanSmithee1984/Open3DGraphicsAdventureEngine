cmake_minimum_required(VERSION 2.8)
project(SkyX)

### Ogre
set(CMAKE_MODULE_PATH
    /usr/lib64/OGRE/cmake/
    ${CMAKE_MODULE_PATH}
)

find_package(OGRE REQUIRED)
include_directories(${OGRE_INCLUDE_DIR})

set(LibDir ${PROJECT_SOURCE_DIR}/SkyX)

set(SourceDir ${LibDir}/Source)
set(HeaderDir ${LibDir}/Include)

# include dirs
include_directories(${SourceDir})
include_directories(${HeaderDir})

file(GLOB_RECURSE SkyXLibSrc ${SourceDir}/*.cpp)
file(GLOB_RECURSE SkyXLibH ${SourceDir}/*.h
                       ${HeaderDir}/*.h)

#set(ShaderPath ${ProjectBaseDir}/data/ogre/media/shader/)
#file(GLOB FragmentShader ${ShaderPath}/*.frag)
#file(GLOB VertexShader ${ShaderPath}/*.vert)
#file(GLOB GeometryShader ${ShaderPath}/*.geom)


add_library(SkyXLib SHARED ${SkyXLibSrc} ${SkyXLibH}
                             ${FragmentShader} ${VertexShader} ${GeometryShader})
TARGET_LINK_LIBRARIES(SkyXLib ${OGRE_LIBRARIES} )

install(TARGETS SkyXLib LIBRARY DESTINATION lib)

set(PublicHeader ${HeaderDir})
INSTALL(DIRECTORY ${PublicHeader}
         DESTINATION include)

file(GLOB Data ${PROJECT_SOURCE_DIR}/SkyXCommon/Bin/Media/SkyX)
INSTALL(DIRECTORY ${Data}
         DESTINATION share)
