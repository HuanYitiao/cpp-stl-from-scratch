include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)

# Prevent GoogleTest from overriding compiler/linker options
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

# Helper function: add a ministl test target
# Usage: ministl_add_test(vector)  ->  creates target "vector_test"
function(ministl_add_test name)
    set(target "${name}_test")
    add_executable(${target} ${target}.cpp)
    target_link_libraries(${target} PRIVATE ministl GTest::gtest_main)
    gtest_discover_tests(${target})
endfunction()

# ─── Register test targets ───────────────────────────────────
# Uncomment as you implement each container:

ministl_add_test(vector)
# ministl_add_test(list)
# ministl_add_test(deque)
# ministl_add_test(stack)
# ministl_add_test(queue)
# ministl_add_test(hashtable)
# ministl_add_test(unordered_set)
# ministl_add_test(unordered_map)
# ministl_add_test(rbtree)
# ministl_add_test(set)
# ministl_add_test(map)
# ministl_add_test(multiset)
# ministl_add_test(multimap)
# ministl_add_test(priority_queue)
