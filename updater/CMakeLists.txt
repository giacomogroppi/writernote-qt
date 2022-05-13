cmake_minimum_required(VERSION 3.5)

project(writernote LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QT_VERSION_MAJOR 6)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC OR MSYS OR MINGW)
    set(WIN TRUE)
endif()

file(GLOB PROJECT_MOC
	
)

file(GLOB PROJECT_UI
    updater.ui
)

file(GLOB PROJECT_SOURCES
    main.cpp
    updater.cpp 
    ../src/utils/dialog_critic/dialog_critic.cpp
    ../src/utils/get_file_dir/get_file_dir.cpp
    ../src/utils/path/get_path.cpp
)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)

qt_add_executable(${PROJECT_NAME} ${PROJECT_MOC} ${PROJECT_SOURCES})


set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
	AUTOUIC TRUE
	AUTOUIC_SEARCH_PATHS "${PROJECT_SOURCE_DIR}"
	AUTOMOC TRUE
	AUTORCC TRUE
	AUTOGEN_SOURCE_GROUP "Qt Files"
)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/../src)

SET (CMAKE_CXX_FLAGS "-Wno-deprecated-declarations -O3")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Network)
