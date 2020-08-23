


add_executable(game 
    main.cpp
    ../include/dungeon.h
    ../fungeon/utils.h ../fungeon/utils.cpp
)

target_link_libraries(game command_line dungeon fungeon)