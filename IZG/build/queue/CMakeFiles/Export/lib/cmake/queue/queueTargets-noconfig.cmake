#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "queue::queue" for configuration ""
set_property(TARGET queue::queue APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(queue::queue PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libqueue.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS queue::queue )
list(APPEND _IMPORT_CHECK_FILES_FOR_queue::queue "${_IMPORT_PREFIX}/lib/libqueue.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
