add_executable(1corr 1corr.c hmload.c rand.c)
add_executable(th th.c hmload.c rand.c)

add_test(1corr 1corr)
add_test(th th)
set_target_properties(1corr PROPERTIES COMPILE_FLAGS -fcommon)
set_target_properties(th PROPERTIES COMPILE_FLAGS -fcommon)
target_link_libraries(1corr ${libs})
target_link_libraries(th ${libs})
