set(CMAKE_INCLUDE_CURRENT_DIR ON)
cmake_policy(SET CMP0087 NEW)
configure_file(appinfo.h.in ${CMAKE_CURRENT_BINARY_DIR}/appinfo.h @ONLY NEWLINE_STYLE UNIX)

qt_add_resources(RESOURCES ../resources/resources.qrc)

qt6_add_executable(${PROJECT_NAME}
        gameboard.h
        gameboard.cpp
        gameStats.cpp
        gameStats.h
        highScore.cpp
        highScore.h
        highScoreDialog.cpp
        highScoreDialog.h
        highScoreModel.cpp
        highScoreModel.h
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mineCounter.h
        mineCounter.cpp
        minetimer.cpp
        minetimer.h
        tile.cpp
        tile.h
        ${RESOURCES}
        ${CMAKE_CURRENT_BINARY_DIR}/appinfo.h
        gameStatsDialog.cpp
        gameStatsDialog.h
)

set(MACOSX_BUNDLE_ICON_FILE "Minesweeper.icns")
set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "Minesweeper"
        MACOSX_BUNDLE_ICON_FILE ${MACOSX_BUNDLE_ICON_FILE}
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/resources/Info.plist"
)

file(COPY "${CMAKE_SOURCE_DIR}/resources/Minesweeper.icns"
        DESTINATION "${CMAKE_BINARY_DIR}/bin/Minesweeper.app/Contents/Resources/")


target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::StateMachine
        Qt::Concurrent
        Qt::Network
        Threads::Threads
)