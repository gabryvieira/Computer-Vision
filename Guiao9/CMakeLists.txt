cmake_minimum_required(VERSION 2.8)
project( Guiao9 )
find_package( OpenCV REQUIRED )
add_executable( ex1 ex1/ex1.cpp )
target_link_libraries( ex1  ${OpenCV_LIBS} )
add_executable( ex2 ex2/ex2.cpp )
target_link_libraries( ex2  ${OpenCV_LIBS} ${OpenCV_CFLAGS} )

