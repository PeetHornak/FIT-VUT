#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "vector::vector" for configuration ""
set_property(TARGET vector::vector APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(vector::vector PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libvector.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS vector::vector )
list(APPEND _IMPORT_CHECK_FILES_FOR_vector::vector "${_IMPORT_PREFIX}/lib/libvector.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
