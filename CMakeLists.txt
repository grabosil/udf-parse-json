cmake_minimum_required(VERSION 2.6)

set(LIBRARY_OUTPUT_PATH "build")
set(EXECUTABLE_OUTPUT_PATH "build")

find_program(CLANG_EXECUTABLE clang++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb -std=gnu++11")

set(IR_COMPILE_FLAGS "-emit-llvm" "-c" "-std=gnu++11")
function(COMPILE_TO_IR SRC_FILE)
  get_filename_component(BASE_NAME ${SRC_FILE} NAME_WE)
  set(OUTPUT_FILE "build/${BASE_NAME}.ll")
  add_custom_command(
    OUTPUT ${OUTPUT_FILE}
    COMMAND ${CLANG_EXECUTABLE} ${IR_COMPILE_FLAGS} ${SRC_FILE} -o ${OUTPUT_FILE}
    DEPENDS ${SRC_FILE})
  add_custom_target(${BASE_NAME}-ir ALL DEPENDS ${OUTPUT_FILE})
endfunction(COMPILE_TO_IR)

add_library(parsejson SHARED parse-json.cc)

if (CLANG_EXECUTABLE)
  COMPILE_TO_IR(parse-json.cc)
endif(CLANG_EXECUTABLE)

target_link_libraries(parsejson ImpalaUdf)
add_executable(udf_json_parse_test udf_json_parse_test.cc)
target_link_libraries(udf_json_parse_test parsejson)
