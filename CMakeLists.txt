cmake_minimum_required(VERSION 3.25) # Need 3.25+ for better C++20 modules support
project(Ticks VERSION 0.0.1 LANGUAGES CXX)

# License information
set(PROJECT_LICENSE "LGPL-3.0")
set(PROJECT_LICENSE_URL "https://www.gnu.org/licenses/lgpl-3.0.en.html")

# Install license files
install(FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
    "${CMAKE_CURRENT_SOURCE_DIR}/COPYING"
    DESTINATION "${CMAKE_INSTALL_DOCDIR}"
)

# Use C++20 standard with modules support
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable C++20 modules support with compiler-specific flags
if(MSVC)
  add_compile_options(/W4 /experimental:module /std:c++20)
  # MSVC-specific module options
  add_compile_options(/interface)
  add_compile_options(/EHsc)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -fmodules -fmodules-ts)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Wall -Wextra -Wpedantic -fmodules-ts)
endif()

# Find dependencies
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

find_package(yaml-cpp REQUIRED)

# Module files
file(GLOB_RECURSE MODULE_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/modules/*.ixx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/modules/*.cppm"
)

# Implementation files
file(GLOB_RECURSE IMPL_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/impl/*.cpp"
)

# Set module build directory
set(CMAKE_CXX_MODULES_DIRECTORY ${CMAKE_BINARY_DIR}/modules)

# Main executable with modules
add_executable(${PROJECT_NAME} 
    main.cppm 
    ${MODULE_FILES}
    ${IMPL_FILES}
)

# Set module-specific properties for MSVC
if(MSVC)
  set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    VS_GLOBAL_EnableModules "true"
  )
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES} yaml-cpp)

# Copy configuration
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config/default_config.yaml
    ${CMAKE_CURRENT_BINARY_DIR}/config/default_config.yaml
    COPYONLY
)

# Tests
option(BUILD_TESTS "Build the tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()