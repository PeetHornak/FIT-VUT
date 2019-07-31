cmake_minimum_required(VERSION 3.12.0)

PROJECT(izgProject)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_C_STANDARD 99)

set(SOURCES 
  student/main.c
  student/gpu.c
  student/gpu.h
  student/cpu.c
  student/cpu.h
  student/fwd.h
  student/linearAlgebra.c
  student/linearAlgebra.h
  student/drawPoints.cpp
  student/drawTriangles.cpp
  student/window.h
  student/window.c
  student/application.h
  student/application.c
  student/callbackAndData.h
  student/callbackAndData.c
  student/method.h
  student/method.c
  student/emptyMethod.h
  student/emptyMethod.c
  student/pointMethod.h
  student/pointMethod.c
  student/pointCircleMethod.h
  student/pointCircleMethod.c
  student/pointSquareMethod.h
  student/pointSquareMethod.c
  student/pointBoxMethod.h
  student/pointBoxMethod.c
  student/phongMethod.h
  student/phongMethod.c
  student/triangleMethod.h
  student/triangleMethod.c
  student/triangle3DMethod.h
  student/triangle3DMethod.c
  student/parseArguments.h
  student/parseArguments.c
  student/bunny.h
  student/bunny.c
  student/globals.h
  student/globals.c
  student/camera.h
  student/camera.c
  tests/conformanceTests.cpp
  tests/conformanceTests.h
  tests/performanceTest.h
  tests/performanceTest.cpp
  tests/takeScreenShot.h
  tests/takeScreenShot.cpp
  tests/groundTruth.c
  tests/groundTruth.h
  tests/tests.cpp
  )

find_package(SDL2 2.0.9 CONFIG REQUIRED)

add_subdirectory(errorCodes)
add_subdirectory(stack)
add_subdirectory(queue)
add_subdirectory(vector)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} SDL2::SDL2 SDL2::SDL2main stack::stack queue::queue vector::vector)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

option(${PROJECT_NAME}_BUILD_INTERNAL_TESTS "toggle building of internal tests")
if(${PROJECT_NAME}_BUILD_INTERNAL_TESTS)
  add_subdirectory(tests/internalTests)
endif()

function(getSharedLibraries out lib cfg)
  #message("getSharedLibraries(${lib} ${cfg})")

  if(TARGET ${lib})
    get_target_property(target_type ${lib} TYPE)
    if(${target_type} STREQUAL "INTERFACE_LIBRARY")
      #message("${lib} JE ${target_type}")
      return()
    endif()
  else()
    #message("${lib} NENI TARGET")
    return()
  endif()


  get_target_property(dll ${lib} IMPORTED_LOCATION_${cfg})

  string(REGEX MATCH "\\.dll$" isDll ${dll})
  if("${isDll}" STREQUAL "")
    return()
  endif()

  #message("${lib} ---- ${dll}")
  list(APPEND dlls ${dll})

  get_target_property(interfaceLibs ${lib} INTERFACE_LINK_LIBRARIES)
  if(NOT "${interfaceLibs}" STREQUAL "interfaceLibs-NOTFOUND")
    foreach(interfaceLib ${interfaceLibs})
      getSharedLibraries(ilibs ${interfaceLib} ${cfg})
      list(APPEND dlls ${ilibs})
    endforeach()
  endif()
  list(REMOVE_DUPLICATES dlls)
  set(${out} ${dlls} PARENT_SCOPE)
endfunction()


function(getAllSharedLibraries allLibraries app cfg)
  get_target_property(libs ${app} LINK_LIBRARIES)
  foreach(lib ${libs})
    getSharedLibraries(libList ${lib} ${cfg})
    #message("${lib} ###### ${libList}")
    list(APPEND allLibs ${libList})
  endforeach()
  list(REMOVE_DUPLICATES allLibs)
  set(${allLibraries} ${allLibs} PARENT_SCOPE)
endfunction()

if(MSVC)
  getAllSharedLibraries(allDebugSharedLibraries ${PROJECT_NAME} DEBUG)
  getAllSharedLibraries(allReleaseSharedLibraries ${PROJECT_NAME} RELEASE)
  
  #message("SharedDebug  : ${allDebugSharedLibraries}")
  #message("SharedRelease: ${allReleaseSharedLibraries}")

  foreach(lib ${allDebugSharedLibraries})
    file(COPY ${lib} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
  endforeach()
  foreach(lib ${allReleaseSharedLibraries})
    file(COPY ${lib} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
  endforeach()
endif() 

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)
install(FILES ${allDebugSharedLibraries} DESTINATION .)
