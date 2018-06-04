
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,\$ORIGIN")

link_libraries(dex)
add_executable(leak leak.cpp)
add_executable(resource resource.cpp)

install(TARGETS leak DESTINATION ./)
install(TARGETS resource DESTINATION ./)