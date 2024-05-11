cmake_minimum_required(VERSION 3.27)
project(Mirio)

set(CMAKE_CXX_STANDARD 17)



cmake_minimum_required(VERSION 3.19)
project(Mirio C)
set(CMAKE_C_STANDARD 99)
add_executable(Mirio




        AllegroBase.cpp


        mirio.cpp


)
target_link_libraries (Mirio allegro allegro_primitives )
