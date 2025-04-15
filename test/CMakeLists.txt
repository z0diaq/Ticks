# Find GTest
find_package(GTest REQUIRED)

# Get test module files
file(GLOB_RECURSE TEST_MODULE_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/model/*.ixx"
    "${CMAKE_CURRENT_SOURCE_DIR}/model/*.cppm"
)

# Get test implementation files 
file(GLOB_RECURSE TEST_IMPL_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/model/*.cpp"
)

# Create test executable
add_executable(${PROJECT_NAME}_test 
    test_main.cppm
    ${TEST_MODULE_FILES}
    ${TEST_IMPL_FILES}
    ${MODULE_FILES}
    ${IMPL_FILES}
)

# Set module-specific properties for MSVC
if(MSVC)
  set_target_properties(${PROJECT_NAME}_test PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    VS_GLOBAL_EnableModules "true"
  )
endif()

# Set include directories and link libraries
target_include_directories(${PROJECT_NAME}_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME}_test PRIVATE 
    ${wxWidgets_LIBRARIES}
    yaml-cpp::yaml-cpp
    GTest::GTest
    GTest::Main
)

# Add test
add_test(NAME ${PROJECT_NAME}_test COMMAND ${PROJECT_NAME}_test)