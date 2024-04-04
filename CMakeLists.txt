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
    src/main.cc

    src/view/view.cc
    src/view/view.hpp
    src/view/view.ui
    src/view/graphic_widget.hpp

    src/model/games/game.hpp

    src/model/games/snake/snake.hpp
    src/model/games/tetris/tetris.hpp

    src/controller/controller.hpp
    src/model/facade/model_facade.hpp
)

qt_add_executable(BrickGame
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(BrickGame PRIVATE Qt6::Widgets)

include_directories(
    src/view
    src/model/facade
    src/model/games
    src/model/games/snake
    src/model/games/tetris
    src/controller
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
