cmake_minimum_required(VERSION 3.2)

PROJECT(renderer)

if (CMAKE_HOST_WIN32)
    set(WINDOWS 1)
elseif(CMAKE_HOST_UNIX)
    set(LINUX 1)
endif()

set(CMAKE_CXX_STANDARD 11)

if(WINDOWS)
    INCLUDE_DIRECTORIES(${BOOST})
elseif(LINUX)
    find_package(Boost REQUIRED COMPONENTS)

    INCLUDE_DIRECTORIES(${BOOST_INCLUDE_DIR})
endif()

INCLUDE_DIRECTORIES(../src ../include)

AUX_SOURCE_DIRECTORY(../src DIR_SRCS)

LINK_DIRECTORIES(../lib)

if(WINDOWS)
    LINK_DIRECTORIES(${BOOST})
elseif(LINUX)
    LINK_DIRECTORIES(/usr/local/lib)
endif()

ADD_EXECUTABLE(${PROJECT_NAME} ${DIR_SRCS})

if(WINDOWS)
    target_link_libraries(${PROJECT_NAME} glew32s.lib libglfw3dll.lib OPENGL32.LIB freetype.lib)
elseif(LINUX)
    target_link_libraries(${PROJECT_NAME} ${BOOST_LIBRARY})

    target_link_libraries(${PROJECT_NAME} libglfw3.a libGLEW.a)
    target_link_libraries(${PROJECT_NAME} -lGL -lX11 -lXrandr -lXcursor -lXxf86vm -lXinerama -lpthread -ldl -lfreetype -lm)
endif()

if(WINDOWS)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/freetype6.dll" $<TARGET_FILE_DIR:renderer>)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/glfw3.dll" $<TARGET_FILE_DIR:renderer>)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/res" $<TARGET_FILE_DIR:renderer>/res)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/shaders" $<TARGET_FILE_DIR:renderer>/shaders)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/res" $<TARGET_FILE_DIR:renderer>/../res)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/shaders" $<TARGET_FILE_DIR:renderer>/../shaders)
endif()


