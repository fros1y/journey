file(GLOB journey_SRC *.cpp)

include_directories(include ${entityx_INCLUDE_DIR})

add_executable(Journey ${journey_SRC})

target_link_libraries(Journey ${entityx_LIBRARY} "/usr/local/lib/libtcod.dylib")
