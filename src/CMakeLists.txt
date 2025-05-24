set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_add_resources(RESOURCES ../resources/resources.qrc)

qt6_add_executable(${PROJECT_NAME}
        gameboard.h
        gameboard.cpp
        ${RESOURCES}
)