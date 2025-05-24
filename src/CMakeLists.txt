set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_add_resources(RESOURCES ../resources/resources.qrc)

qt6_add_executable(${PROJECT_NAME}
        gameboard.h
        gameboard.cpp
        minetimer.h
        minetimer.cpp
        ${RESOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::StateMachine
        Qt::Concurrent
        Qt::Network
        Threads::Threads
)