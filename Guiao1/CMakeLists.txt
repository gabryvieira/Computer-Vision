cmake_minimum_required(VERSION 2.8)
project( Guiao2 )
find_package( OpenCV REQUIRED )
add_executable( ex3 Aula2_ex3/main.cpp )
target_link_libraries( ex3  ${OpenCV_LIBS} )
add_executable( ex4 aula2_ex4/main.cpp )
target_link_libraries( ex4  ${OpenCV_LIBS} )
add_executable( ex5 aula2_ex5/main.cpp )
target_link_libraries( ex5  ${OpenCV_LIBS} )
add_executable( ex6 aula2_ex6/main.cpp )
target_link_libraries( ex6  ${OpenCV_LIBS} )
add_executable( ex7 aula2_ex7/main.cpp )
target_link_libraries( ex7  ${OpenCV_LIBS} )
add_executable( ex7_alt aula2_ex7/main_alt.cpp )
target_link_libraries( ex7_alt  ${OpenCV_LIBS} )
