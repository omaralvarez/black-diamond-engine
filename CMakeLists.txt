PROJECT(BDM C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.7)

FIND_PACKAGE(PNG REQUIRED)
ADD_DEFINITIONS(${PNG_DEFINITIONS})
INCLUDE_DIRECTORIES(${PNG_INCLUDE_DIRS})
LINK_LIBRARIES(${PNG_LIBRARIES})

SET(Boost_USE_MULTITHREADED ON)
FIND_PACKAGE(Boost 1.48 COMPONENTS system chrono timer REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_LIBRARIES(${Boost_LIBRARIES})

FIND_PACKAGE(OpenMP REQUIRED)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

SET(SRC_MATH
    src/math/matrix4x4.cpp
    src/math/point.cpp
    src/math/transform.cpp
    src/math/vector.cpp
)

SET(SRC_TOOLS
    src/tools/tinystr.cpp
    src/tools/tinyxml.cpp
    src/tools/tinyxmlerror.cpp
    src/tools/tinyxmlparser.cpp
)

SET(SRC_MAIN
    src/aggregate.cpp
    src/bbox.cpp
    src/bde.cpp
	src/bdesettings.cpp
    src/camera.cpp
    src/image.cpp
    src/kdtreeaccel.cpp
    src/light.cpp
	src/material.cpp
    src/memoryarena.cpp
	src/montecarlo.cpp
    src/parser.cpp
	src/pointlight.cpp
    src/ray.cpp
    src/render.cpp
	src/sampler.cpp
    src/scene.cpp
    src/surfel.cpp
)

INCLUDE_DIRECTORIES(src src/math src/tools)

ADD_LIBRARY(bdemath ${SRC_MATH})
ADD_EXECUTABLE(bde ${SRC_MAIN} ${SRC_TOOLS})

TARGET_LINK_LIBRARIES(bde bdemath)


