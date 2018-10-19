project(${PROJECT_LIB} VERSION 1.0.0)

set(CMAKE_INSTALL_PREFIX /usr/local/${PROJECT_LIB})

include_directories (
        ../include
        ../vendor/crypto
        ../vendor/ed25519/src
        ../vendor/nlohmann/
)

FILE(GLOB INCLUDE_FILES
        ../vendor/nlohmann/*.hpp
        ../vendor/crypto/*.h
        ../vendor/crypto/*.hpp
        ../vendor/ed25519/src/*.h
        ../include/*.hpp
        )

FILE(GLOB SOURCES
        ../vendor/crypto/*.cpp
        ../vendor/crypto/*.c
        ../vendor/ed25519/src/*.c
        ../src/*.cpp
        ../src/transactions_wrapper/*.cpp
        )

set (BOOST_COMPONENTS system)

find_package (Boost  REQUIRED COMPONENTS ${BOOST_COMPONENTS})
include_directories(${Boost_INCLUDE_DIRS})

if (MSVC)
    add_definitions (/W4)
elseif (CMAKE_COMPILER_IS_GNUCXX)
    add_definitions (-Wall -pedantic)
else ()
    message ("Unknown compiler")
endif ()

add_library(${PROJECT_LIB}  ${SOURCES})

target_link_libraries (
        ${PROJECT_LIB}  PUBLIC
        ${Boost_LIBRARIES}
)

target_include_directories(
        ${PROJECT_LIB}
        PUBLIC
        ../include
        ../vendor/nlohmann
        ../vendor/crypto
)

set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

# Configuration
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}")
set(namespace "${PROJECT_NAME}::")

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        "${version_config}" COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
        "cmake/Config.cmake.in"
        "${project_config}"
        INSTALL_DESTINATION "${config_install_dir}"
)

install(FILES ${INCLUDE_FILES} DESTINATION include)
install(TARGETS ${PROJECT_LIB}
        DESTINATION lib)
install(FILES ${project_config} ${version_config} DESTINATION lib/cmake/milecsa)

install(
        FILES "${project_config}" "${version_config}"
        DESTINATION "${config_install_dir}"
)

install(FILES cmake/${PROJECT_LIB}.cmake DESTINATION lib/cmake/milecsa)