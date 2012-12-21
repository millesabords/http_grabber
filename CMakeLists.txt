cmake_minimum_required (VERSION 2.6)
project (http_grabber)
#add_definitions(-std=c99)

set(CMAKE_CXX_FLAGS "-g -Wall")
add_subdirectory(src)

# makefile rule for tests: ./binary/http_grabber www.qtcentre.org /threads/3514-CMake-and-pthreads > test/resultattata.html
# makefile rules for tar, compiles, moulette

find_package (Threads)
#include(FindBoost)
find_package(
	Boost
	REQUIRED system
)
if ( NOT Boost_FOUND )
    message(FATAL_ERROR "Boost not found !")
endif ( NOT Boost_FOUND )
set(Boost_USE_MULTITHREAD OFF)

MESSAGE( STATUS "boost link line=" ${Boost_LIBRARIES})




# CPack archiving system
# to build the rule "make package_source" or "make dist"
# or use directly git to do it this way:
# git archive --prefix=mysoftware-$version . | bzip2

set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_SOURCE_GENERATOR "TBZ2")
set(CPACK_SOURCE_PACKAGE_FILE_NAME
  "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_SOURCE_IGNORE_FILES
  "/build/;/.git/;~$;${CPACK_SOURCE_IGNORE_FILES};CMakeCache.txt")
include(CPack)
add_custom_target(
dist
COMMAND ${CMAKE_MAKE_PROGRAM} package_source
COMMAND mv "${CPACK_SOURCE_PACKAGE_FILE_NAME}.tar.bz2" ~/Saves/
)
#http_grabber-0.1.0.tar.bz2