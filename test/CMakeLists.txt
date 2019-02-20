project(ut_state)

set(UT_SOURCES
    ut_state.cpp
)

mark3_add_executable(ut_state ${UT_SOURCES})

target_link_libraries(ut_state.elf
    ut_base
    mark3
    state_machine
    memutil
)
