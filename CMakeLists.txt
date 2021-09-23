cmake_minimum_required(VERSION 2.8.12)
project(critnib C)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
include(FindThreads)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif (NOT CMAKE_BUILD_TYPE)

set(CMAKE_C_FLAGS "-Wall -Werror=format-security ${CMAKE_C_FLAGS}")

add_library(critnib SHARED critnib.c)
set(libs critnib ${CMAKE_THREAD_LIBS_INIT})
add_executable(m main.c)
target_link_libraries(m ${libs})

set_target_properties(critnib PROPERTIES
	SOVERSION 1
	PUBLIC_HEADER "critnib.h")

include(GNUInstallDirs)
install(TARGETS critnib
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}/)
install(FILES critnib.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES
	libcritnib.3
	critnib.3
	DESTINATION share/man/man3)

enable_testing()
add_subdirectory(tests)

add_custom_target(man ALL
	BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/libcritnib.3
	COMMENT "Building libcritnib.3"
	COMMAND pod2man -c critnib -s 3 -n libcritnib
		<${CMAKE_CURRENT_SOURCE_DIR}/libcritnib.pod
		>${CMAKE_CURRENT_BINARY_DIR}/libcritnib.3
	)
