# Created by the script cgal_create_CMakeLists
# This is the CMake script for compiling a set of CGAL applications.

cmake_minimum_required(VERSION 3.1...3.15)

project( uvacpp )


# CGAL and its components
find_package( CGAL QUIET COMPONENTS  )

if ( NOT CGAL_FOUND )

  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()  

endif()


# Boost and its components
find_package( Boost REQUIRED )

if ( NOT Boost_FOUND )

  message(STATUS "This project requires the Boost library, and will not be compiled.")

  return()  

endif()
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
# GDAL and its components

find_package(GDAL REQUIRED)
if ( NOT GDAL_FOUND )

  message(STATUS "This project requires the GDAL library, and will not be compiled.")

  return()  

endif()
include_directories(${GDAL_INCLUDE_DIRS})

# include for local directory
# include for local package
include(${CGAL_USE_FILE})
# Creating entries for all C++ files with "main" routine
# ##########################################################
add_executable(main test.cpp)
target_link_libraries(main ${GDAL_LIBRARY} ${CGAL_LIBS})




