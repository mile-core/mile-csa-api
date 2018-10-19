add_library(milecsa STATIC IMPORTED)


find_library(milecsa_LIBRARY_PATH milecsa HINTS "${CMAKE_CURRENT_LIST_DIR}/../../")
set_target_properties(milecsa PROPERTIES IMPORTED_LOCATION "${milecsa_LIBRARY_PATH}")

include_directories(
        "${milecsa_INCLUDE_PATH}"
)

message(STATUS "CMAKE_CURRENT_LIST_DIR "  ${CMAKE_CURRENT_LIST_DIR})
message(STATUS "CMAKE_INSTALL_PREFIX " ${CMAKE_INSTALL_PREFIX})
message(STATUS "milecsa_LIBRARY_PATH " ${milecsa_LIBRARY_PATH})
message(STATUS "milecsa_INCLUDE_PATH " ${milecsa_INCLUDE_PATH})
