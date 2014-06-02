IF(CMAKE_BUILD_TYPE MATCHES Release)
    ADD_DEFINITIONS(-DRELEASE)
    ADD_DEFINITIONS(-DQT_NO_DEBUG_OUTPUT)
    ADD_DEFINITIONS(-DQT_NO_DEBUG)

    if(CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "-O1 -Wreturn-type  -Wall -Wextra -Wno-long-long -Wno-unused -Wno-ignored-qualifiers -Wno-unknown-pragmas")  ## -Werror -pedantic (deactivated due to hydrax)
        #set(CMAKE_CXX_FLAGS "-O1 -pedantic-errors -Wreturn-type -Werror -Wall -Wextra -Wno-unused -Wno-long-long")  ## Optimize
        #set(CMAKE_CXX_FLAGS "-O1 -Wreturn-type -fpermissive -Wl,--no-as-needed -Wl,--no-undefined")  ## Optimize
    endif()

    message("Info: Debugging disabled -> release build")

ELSE(CMAKE_BUILD_TYPE MATCHES Release)

# debug build
    set(CMAKE_BUILD_TYPE Debug)
    ADD_DEFINITIONS(-DDEBUG)
    ADD_DEFINITIONS(-D_DEBUG)

    #ADD_DEFINITIONS(-DQT_NO_DEBUG_OUTPUT)
    #ADD_DEFINITIONS(-DQT_NO_DEBUG)
    set(CMAKE_CXX_FLAGS "-O0  -Wreturn-type -Wall -Wextra -Wno-long-long -Wno-unused -Wno-ignored-qualifiers -Wno-unknown-pragmas") #   -Werror -pedantic (deactivated due to hydrax)
    #set(CMAKE_CXX_FLAGS "-O0  -pedantic-errors -Wreturn-type -Werror -Wall -Wextra -Wno-unused -Wno-long-long")
    #set(CMAKE_CXX_FLAGS "-O0 -Wreturn-type -fpermissive -Wl,--no-as-needed -Wl,--no-undefined")

    message("Info: Debugging enabled -> debug build")

ENDIF(CMAKE_BUILD_TYPE MATCHES Release)




include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
