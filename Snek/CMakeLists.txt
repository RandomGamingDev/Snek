cmake_minimum_required (VERSION 3.8)

find_package(fmt CONFIG REQUIRED)

add_executable (Snek  
	Main.cpp
	Pixy.h
	GameObjects.h
)

target_link_libraries(Snek PRIVATE fmt::fmt)

target_include_directories(Snek PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Wireframe)
target_link_libraries(Snek PRIVATE Wireframe)
