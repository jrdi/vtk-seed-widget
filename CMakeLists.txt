cmake_minimum_required(VERSION 2.8)
 
PROJECT(widget)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(widget MACOSX_BUNDLE widget)
 
if(VTK_LIBRARIES)
  target_link_libraries(widget ${VTK_LIBRARIES})
else()
  target_link_libraries(widget vtkHybrid)
endif()
