cmake_minimum_required (VERSION 2.6)
project (SM)

set(INCLUDE_DIRS ${PROJECT_SOURCE_DIR}) 
set(EXECUTABLES main.cpp crypt.cpp) 
set(TARGET_LINK_LIBS) 

list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

### BOOST library
	set(Boost_USE_STATIC_LIBS        OFF)
	set(Boost_USE_MULTITHREADED      ON)
	set(Boost_USE_STATIC_RUNTIME     OFF)
	set(BOOST_ALL_DYN_LINK           ON)   # force dynamic linking for all libraries
	set(REQUIRED_BOOST_LIBRARIES system program_options filesystem)
	find_package(Boost 1.49.0 COMPONENTS ${REQUIRED_BOOST_LIBRARIES} REQUIRED)
	if(Boost_FOUND)
	    set(INCLUDE_DIRS ${INCLUDE_DIRS} ${Boost_INCLUDE_DIRS}) 
	    set(TARGET_LINK_LIBS ${TARGET_LINK_LIBS} ${Boost_LIBRARIES}) 
	endif()
###

### SQLiteCpp library
	add_subdirectory(./3rdparty/SQLiteCpp/)
	set(INCLUDE_DIRS ${INCLUDE_DIRS} ./3rdparty/SQLiteCpp/include) 
	set(TARGET_LINK_LIBS ${TARGET_LINK_LIBS} SQLiteCpp sqlite3 pthread dl) 
###

### Crypto++ library
	#find_package(CryptoPP REQUIRED)
	FIND_LIBRARY(CRYPTOPP crypto++ /usr/lib)
	if(CRYPTOPP)
		set(TARGET_LINK_LIBS ${TARGET_LINK_LIBS} cryptopp) 
	endif()
###


### mongoose-Cpp library
	add_subdirectory(./3rdparty/mongoose-cpp/)
	set(INCLUDE_DIRS ${INCLUDE_DIRS} ./3rdparty/mongoose-cpp) 
	set(TARGET_LINK_LIBS ${TARGET_LINK_LIBS} mongoose) 
###
    
### JsonCpp library
	add_subdirectory(./3rdparty/jsoncpp/)
	set(INCLUDE_DIRS ${INCLUDE_DIRS} ./3rdparty/jsoncpp) 
	set(TARGET_LINK_LIBS ${TARGET_LINK_LIBS} jsoncpp)
#	FIND_LIBRARY(JSONCPP jsoncpp /usr/lib)
#	if(JSONCPP)
#		message( "JSONPP FOUND !" )
#		set(TARGET_LINK_LIBS ${TARGET_LINK_LIBS} jsoncpp) 
#	else(JSONCPP)
#		message( "   JSONPP NOT FOUND !!!!!!!!!!!!!!!!!!!!!!!   " )	
#	endif(JSONCPP)
###

    
include_directories(${INCLUDE_DIRS}) 
add_executable(sm ${EXECUTABLES})
### Add C++11 standard

set_property(TARGET sm PROPERTY CXX_STANDARD 11)
set_property(TARGET sm PROPERTY CXX_STANDARD_REQUIRED ON)
#if (${CMAKE_EXTRA_GENERATOR} MATCHES "Eclipse CDT4")
	add_definitions (-DCMAKE_CXX_COMPILER_ARG1=-std=c++11)
	add_definitions (-DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE)
#endif (${CMAKE_EXTRA_GENERATOR} MATCHES "Eclipse CDT4")
###
target_link_libraries(sm ${TARGET_LINK_LIBS})