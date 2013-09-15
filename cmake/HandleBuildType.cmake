IF(CMAKE_BUILD_TYPE MATCHES Release)
    ADD_DEFINITIONS(-DRELEASE)
    ADD_DEFINITIONS(-DQT_NO_DEBUG_OUTPUT)
    ADD_DEFINITIONS(-DQT_NO_DEBUG)

    if(CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "-O1 -Wreturn-type -Werror -Wall -Wextra -pedantic -Wno-long-long -Wno-unused")  ## Optimize  -Wall
        #set(CMAKE_CXX_FLAGS "-O1 -pedantic-errors -Wreturn-type -Werror -Wall -Wextra -Wno-unused -Wno-long-long")  ## Optimize
        #set(CMAKE_CXX_FLAGS "-O1 -Wreturn-type -fpermissive -Wl,--no-as-needed -Wl,--no-undefined")  ## Optimize
    endif()

    message("Info: Debugging disabled -> release build")

ELSE(CMAKE_BUILD_TYPE MATCHES Release)

    set(CMAKE_BUILD_TYPE Debug)
    ADD_DEFINITIONS(-DDEBUG)
    #ADD_DEFINITIONS(-DQT_NO_DEBUG_OUTPUT)
    #ADD_DEFINITIONS(-DQT_NO_DEBUG)
    set(CMAKE_CXX_FLAGS "-O0  -Wreturn-type -Werror -Wall -Wextra -pedantic -Wno-long-long -Wno-unused") #  -Wall
    #set(CMAKE_CXX_FLAGS "-O0  -pedantic-errors -Wreturn-type -Werror -Wall -Wextra -Wno-unused -Wno-long-long")
    #set(CMAKE_CXX_FLAGS "-O0 -Wreturn-type -fpermissive -Wl,--no-as-needed -Wl,--no-undefined")

    message("Info: Debugging enabled -> debug build")

ENDIF(CMAKE_BUILD_TYPE MATCHES Release)
