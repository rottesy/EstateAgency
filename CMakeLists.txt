cmake_minimum_required(VERSION 3.16)
project(EstateAgency VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
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
    # Entities
    src/entities/Property.cpp
    src/entities/Apartment.cpp
    src/entities/House.cpp
    src/entities/CommercialProperty.cpp
    src/entities/Client.cpp
    src/entities/Transaction.cpp
    src/entities/Bid.cpp
    src/entities/Auction.cpp
    # Services
    src/services/PropertyManager.cpp
    src/services/ClientManager.cpp
    src/services/TransactionManager.cpp
    src/services/AuctionManager.cpp
    src/services/FileManager.cpp
    # Core
    src/core/EstateAgency.cpp
    # UI
    src/ui/MainWindow.cpp
    src/ui/PropertyDialog.cpp
    src/ui/ClientDialog.cpp
    src/ui/TransactionDialog.cpp
    src/ui/AuctionDialog.cpp
    src/ui/HtmlFormatter.cpp
    src/ui/PropertiesWidget.cpp
    src/ui/ClientsWidget.cpp
    src/ui/TransactionsWidget.cpp
    src/ui/AuctionsWidget.cpp
    src/ui/DashboardWidget.cpp
    src/ui/TableHelper.cpp
)

# Заголовочные файлы для MOC
set(HEADERS
    include/ui/MainWindow.h
    include/ui/PropertyDialog.h
    include/ui/ClientDialog.h
    include/ui/TransactionDialog.h
    include/ui/AuctionDialog.h
    include/ui/PropertiesWidget.h
    include/ui/ClientsWidget.h
    include/ui/TransactionsWidget.h
    include/ui/AuctionsWidget.h
    include/ui/DashboardWidget.h
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
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/entities)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/services)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/core)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/ui)

# Для сборки в Release
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

# Вывод информации
message(STATUS "Building ${PROJECT_NAME} version ${PROJECT_VERSION}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

