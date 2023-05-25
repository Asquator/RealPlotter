project(RPtest)

add_executable(driver tester.cpp)

target_include_directories(driver
	PRIVATE ${CMAKE_SOURCE_DIR}/lib/RealFunctioLib/include
)

target_link_libraries(driver
	PRIVATE RealFunctionLib
)
