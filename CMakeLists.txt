cmake_minimum_required(VERSION 3.5)

project(BrickGame VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -O3")

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
    main.cc

    src/gui/desktop/view.cc
    src/gui/desktop/view.hpp
    src/gui/desktop/view.ui

    src/gui/desktop/graphic_widget.hpp

    src/brick_game/game.hpp
    src/brick_game/game_info.hpp
    src/brick_game/snake/snake.hpp
    src/brick_game/tetris/tetris.hpp

    src/controller/controller.hpp
    src/brick_game/facade/model_facade.hpp
)

qt_add_executable(BrickGame
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(BrickGame PRIVATE Qt6::Widgets)

include_directories(
    src/controller
    src/gui/desktop
    src/brick_game
    src/brick_game/snake
    src/brick_game/tetris
    src/brick_game/facade
)

set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.BrickGame)
set_target_properties(BrickGame PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS BrickGame
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
