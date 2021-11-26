function(test name)
	add_executable(${name} ${name}.c hmload.c rand.c)
	add_test(${name} ${name})
	set_target_properties(${name} PROPERTIES COMPILE_FLAGS -fcommon)
	target_link_libraries(${name} ${libs})
endfunction()

test(1corr)
test(inequal)
test(th)

add_executable(iter-cli iter-cli.c)
target_link_libraries(iter-cli ${libs})
add_test(iter ${CMAKE_CURRENT_SOURCE_DIR}/iter)
