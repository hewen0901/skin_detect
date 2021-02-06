#1.cmake verson，指定cmake版本 
cmake_minimum_required(VERSION 3.2)

#2.project name，指定项目的名称，一般和项目的文件夹名称对应
PROJECT(skin_detect_SDK)

option(IOS " set ON COMPILE IOS SDK" ON)

#3.head file path，头文件目录
INCLUDE_DIRECTORIES(
     ${CMAKE_CURRENT_SOURCE_DIR}
     ${CMAKE_CURRENT_SOURCE_DIR}/inc
     ${CMAKE_CURRENT_SOURCE_DIR}/export
 )

find_package (OpenCV 3.4.5 REQUIRED)
if(OpenCV_FOUND)
    INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
    message(STATUS "OpenCV library status:")
    message(STATUS "    version: ${OpenCV_VERSION}")
    message(STATUS "    libraries: ${OpenCV_LIBS}")
    message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
endif()


#4.source directory，APP源文件目录
AUX_SOURCE_DIRECTORY(utils DIR_SRCS)
AUX_SOURCE_DIRECTORY(src DIR_SRCS)
AUX_SOURCE_DIRECTORY(. DIR_SRCS)
AUX_SOURCE_DIRECTORY(test TEST_SRCS)



SET(CMAKE_BUILD_TYPE RELEASE)
##设置编译器版本
SET(CMAKE_C_COMPILER gcc)
SET(CMAKE_CXX_COMPILER g++)
if(CMAKE_COMPILER_IS_GNUCXX)
   add_compile_options(-std=c++11)
   message(STATUS "optional:-std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)

#5.set environment variable
SET(PROC_ALL_FILES ${DIR_SRCS})

SET(TEST_APP Test_app)

#add_library(${PROJECT_NAME} SHARED ${PROC_ALL_FILES}) #编译成动态库
add_library(${PROJECT_NAME} STATIC ${PROC_ALL_FILES}) #编译成静态库
add_executable(${TEST_APP} ${TEST_SRCS})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME})

TARGET_LINK_LIBRARIES(${TEST_APP} ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(${TEST_APP} ${PROJECT_NAME})
