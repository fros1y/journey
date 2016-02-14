file(GLOB journey_SRC *.cpp)

include_directories(include ${entityx_INCLUDE_DIR})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/terminal8x14_gs_ro.png ${CMAKE_CURRENT_BINARY_DIR}/terminal8x14_gs_ro.png COPYONLY)
add_executable(Journey ${journey_SRC} utils.cpp utils.h MapGen.cpp MapGen.hpp)

target_link_libraries(Journey ${entityx_LIBRARY} "/usr/local/lib/libtcod.dylib")
