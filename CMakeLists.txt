cmake_minimum_required(VERSION 2.8.9)
project(critnib C)

include(FindThreads)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif (NOT CMAKE_BUILD_TYPE)

set(CMAKE_C_FLAGS "-Wall -Werror=format-security ${CMAKE_C_FLAGS}")

add_library(critnib SHARED critnib.c)
set(libs critnib ${CMAKE_THREAD_LIBS_INIT})
add_executable(m main.c)
target_link_libraries(m ${libs})

include(GNUInstallDirs)
install(TARGETS critnib
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/)
install(FILES critnib.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

enable_testing()
add_subdirectory(tests)
