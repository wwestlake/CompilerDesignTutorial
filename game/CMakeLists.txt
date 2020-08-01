


add_executable(game 
    main.cpp
    ../include/dungeon.h
)

target_link_libraries(game command_line dungeon)