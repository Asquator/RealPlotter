

set (SOURCES
src/real_math.cpp
src/real_function.cpp
src/binary_operation_function.cpp
src/sum_function.cpp
src/product_function.cpp
src/difference_function.cpp
src/fraction_function.cpp
src/negated_function.cpp
src/special_function.cpp
src/composite_function.cpp
src/power_function.cpp
src/constant_function.cpp


include/RealFunctionLib/real_math.h
include/RealFunctionLib/real_function.h
include/RealFunctionLib/binary_operation_function.h
include/RealFunctionLib/sum_function.h
include/RealFunctionLib/product_function.h
include/RealFunctionLib/difference_function.h
include/RealFunctionLib/fraction_function.h
include/RealFunctionLib/negated_function.h
include/RealFunctionLib/special_function.h
include/RealFunctionLib/composite_function.h
include/RealFunctionLib/power_function.h
include/RealFunctionLib/constant_function.h
include/RealFunctionLib/real_function_base.h
)

add_library(RealFunctionLib ${SOURCES})

target_include_directories(RealFunctionLib
PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/RealFunctionLib
PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src  
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

include_directories(PRIVATE src)


