# Expose the build type as a corresponding preprocessor variable
message(STATUS "ERROR: UNKNOWN BUILD TYPE")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG_BUILD)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DRELEASE_BUILD)
endif()