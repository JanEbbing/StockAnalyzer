## Use compiler debugging flags per default
INCLUDE(CxxFlags)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")

find_package(Qt5Widgets CONFIG REQUIRED)

## Creating Binaries for each *.cxx file
SET(PROJECT_LIBS ${Boost_LIBRARIES} Qt5::Widgets stdc++fs)
SET(PROJECT_BIN ${PROJECT_NAME})
ADD_EXECUTABLE(${PROJECT_BIN} ${MAINFOLDER}/src/main.cxx ${PROJECT_SRCS} PortfolioDevelopment.h BuyAndHoldRandomStrategy.h)
TARGET_LINK_LIBRARIES(${PROJECT_BIN} ${PROJECT_LIBS} Qt5::Widgets Qt5::Charts)
INSTALL(TARGETS ${PROJECT_BIN} DESTINATION bin)
