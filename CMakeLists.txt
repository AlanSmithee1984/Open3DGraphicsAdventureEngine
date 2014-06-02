cmake_minimum_required(VERSION 2.8)
project(Open3DGraphicsAdventureEngine)

message("Using compiler: " ${CMAKE_CXX_COMPILER_ID} )

### include additional cmake scripts
set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH}
    ${PROJECT_SOURCE_DIR}/cmake

)

include(LinkIfNotExists)

set(OgreDataDir ${PROJECT_SOURCE_DIR}/data/ogre)
file(GLOB OgreMediaData ${OgreDataDir}/*)


add_subdirectory(engine)
add_subdirectory(sample)
