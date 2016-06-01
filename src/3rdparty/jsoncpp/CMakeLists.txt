# list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")
add_library(jsoncpp
 jsoncpp.cpp
 json/json.h
)
