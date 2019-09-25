find_package(OpenCV REQUIRED)
add_executable(get_tumo_tokopuyo get_tumo_tokopuyo.cpp)
target_link_libraries(get_tumo_tokopuyo log recognition ${OenCV_LIBS})
