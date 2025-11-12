cmake_minimum_required(VERSION 3.16)
project(EstateAgency VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Поиск Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Включаемые файлы
set(INCLUDE_DIR include)

# Исходные файлы
set(SOURCES
    src/main.cpp
    src/Property.cpp
    src/Apartment.cpp
    src/House.cpp
    src/CommercialProperty.cpp
    src/Client.cpp
    src/Transaction.cpp
    src/Bid.cpp
    src/Auction.cpp
    src/PropertyManager.cpp
    src/ClientManager.cpp
    src/TransactionManager.cpp
    src/AuctionManager.cpp
    src/EstateAgency.cpp
    src/MainWindow.cpp
    src/PropertyDialog.cpp
    src/ClientDialog.cpp
    src/TransactionDialog.cpp
    src/AuctionDialog.cpp
    src/HtmlFormatter.cpp
    src/PropertiesWidget.cpp
    src/ClientsWidget.cpp
    src/TransactionsWidget.cpp
    src/AuctionsWidget.cpp
    src/DashboardWidget.cpp
)

# Заголовочные файлы для MOC
set(HEADERS
    include/MainWindow.h
    include/PropertyDialog.h
    include/ClientDialog.h
    include/TransactionDialog.h
    include/AuctionDialog.h
    include/PropertiesWidget.h
    include/ClientsWidget.h
    include/TransactionsWidget.h
    include/AuctionsWidget.h
    include/DashboardWidget.h
)

# Создание исполняемого файла
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Подключение Qt6
target_link_libraries(${PROJECT_NAME} 
    Qt6::Core
    Qt6::Widgets
)

# Подключение std::filesystem (для некоторых компиляторов требуется явная линковка)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
    target_link_libraries(${PROJECT_NAME} stdc++fs)
endif()

# Установка директории включения
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/${INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Для сборки в Release
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

# Вывод информации
message(STATUS "Building ${PROJECT_NAME} version ${PROJECT_VERSION}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

