#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "errorCodes::errorCodes" for configuration ""
set_property(TARGET errorCodes::errorCodes APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(errorCodes::errorCodes PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/liberrorCodes.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS errorCodes::errorCodes )
list(APPEND _IMPORT_CHECK_FILES_FOR_errorCodes::errorCodes "${_IMPORT_PREFIX}/lib/liberrorCodes.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
