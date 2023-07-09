# ┌──────────────────────────────────────────────────────────────────────────────────────┐
# │                                                                                      │
# │                                CMake Facade                                          │
# │                         * * * * * * * * * * * * *                                    │
# │                                                                                      │
# └──────────────────────────────────────────────────────────────────────────────────────┘
#

cmake_minimum_required(VERSION 3.15)

option(CMAKE_BUILD_TYPE "тип сборки"     OFF)
option(BUILD_TESTING    "use unit-test"          OFF)

set(_VERSION_           "0.0.0" CACHE STRING "Версия из git log")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/")


project(ota-backend 
    LANGUAGES       CXX
    DESCRIPTION     "Over The Air by Backend"
    HOMEPAGE_URL    "https://github.com/vasilievsv/ota-backend"
    VERSION         ${_VERSION_})

# Флаги компилятора
set(CMAKE_CXX_STANDARD          14)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(CMAKE_CXX_FLAGS             "${CMAKE_CXX_FLAGS} -fpermissive")

set(BUILD_SHARED_AND_STATIC_LIBS,ON)

# Настройка глобальны инклудов
#
#set(CMAKE_CXX_FLAGS             "${CMAKE_CXX_FLAGS} -idirafter /usr/include")
#unset(CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES)
#unset(CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES)

##########################################################################################
# BLOCK-0: 
#
#   Внешние либы (c cmake'ом)
#
##########################################################################################


add_subdirectory(libs/rabbitmq-c )
add_subdirectory(libs/restbed )
add_subdirectory(libs/cjson )

#
# !!! принудительно указываем include path
#
include_directories( ${CMAKE_SOURCE_DIR}/libs/restbed/distribution/include )


#
# hotfix: поиск static либы в первую очередь
#
SET(CMAKE_FIND_LIBRARY_SUFFIXES .a)

#find_library( MY_STATIC_RABBITMQ ${PROJECT_NAME} 
#                PATHS 
#                    ${CMAKE_SOURCE_DIR}/libs/rabbitmq-c/build/librabbitmq/ 
#                NAMES rabbitmq )

find_library( MY_STATIC_RESTBED ${PROJECT_NAME} 
                PATHS 
                    ${CMAKE_SOURCE_DIR}/libs/restbed/distribution/library
                NAMES restbed )

find_library( MY_STATIC_CJSON ${PROJECT_NAME} 
                PATHS 
                    ${CMAKE_SOURCE_DIR}/libs/cjson/build
                NAMES cjson )


message( "FOUND_RABBITMQ_STATIC_LIB" ${MY_STATIC_RABBITMQ})
message( "FOUND_RESTBED_STATIC_LIB"  ${MY_STATIC_RESTBED})
message( "FOUND_JSON_STATIC_LIB"     ${MY_STATIC_CJSON})

##########################################################################################
#   
#   
#    
#   
#   
##########################################################################################

file(GLOB FILES_LIBS libs/my_utils/*.cpp libs/my_utils/*.h)

add_library( _this_ota_backend_utils STATIC ${FILES_LIBS})

#
# !!! Объектник библиотеки
#
target_link_libraries(_this_ota_backend_utils 
                        LINK_PRIVATE 
                            rabbitmq  cjson pthread cpprest crypto ssl )
#
# Настройка инклудов для сборки _this_project_common
# 
# PRIVATE       : The includes can only be used by the helpers-library itself.
# PUBLIC        : The includes can be used by the helpers-library itself and any target that 
#                   uses the helpers-library, e.g. via target_link_libraries(MainApplication PUBLIC libhelpers).
# INTERFACE     : The includes can't be used by the helpers-library, only by targets that use the helpers-library.
# SYSTEM PRIVATE:
#
target_include_directories( _this_ota_backend_utils
                            #SYSTEM PRIVATE 
                            #    /usr/include 
                            #    "/usr/include/cpprest"
                            PUBLIC 
                                "src"
                                "src/backend"
                                "libs"
                                "libs/cJSON"
                                "libs/sdk"
                                "$<TARGET_PROPERTY:rabbitmq,INTERFACE_INCLUDE_DIRECTORIES>"
                            INTERFACE
                                "$<INSTALL_INTERFACE:include>"
                                )

##########################################################################################
# BLOCK-1: 
#   
#   lib_this_project_common.a
#   lib_this_project_common.so
#
#   Собираем основной код без main.cpp
#
##########################################################################################

#
#   With the GLOB_RECURSE command, it is more tricky.
#   https://cmake.org/pipermail/cmake/2007-August/015837.html
#
file(GLOB_RECURSE FILE_SRC src/*.cpp src/*.h)
foreach(item ${FILE_SRC})
  if(${item} MATCHES "main.cpp")
    list(REMOVE_ITEM FILE_SRC ${item})
  endif(${item} MATCHES "main.cpp")
endforeach(item)
list(REMOVE_ITEM FILE_SRC "src/main.cpp" )  # Убираем main.cpp (собирается отдельно)
message(STATUS "СПИСОК ФАЙЛОВ ДЛЯ СБОРКИ \n ${FILE_SRC}")


#
# !!! Объектник библиотеки
#
add_library             ( _this_project_common STATIC ${FILE_SRC})
target_link_libraries   ( _this_project_common 
                            LINK_PRIVATE 
                                 ${MY_STATIC_RESTBED} _this_ota_backend_utils cpprest cjson)

#
# Настройка инклудов для сборки _this_project_common
# 
# PRIVATE       : The includes can only be used by the helpers-library itself.
# PUBLIC        : The includes can be used by the helpers-library itself and any target that 
#                   uses the helpers-library, e.g. via target_link_libraries(MainApplication PUBLIC libhelpers).
# INTERFACE     : The includes can't be used by the helpers-library, only by targets that use the helpers-library.
# SYSTEM PRIVATE:
#
target_include_directories( _this_project_common
                            #SYSTEM PRIVATE 
                            #    /usr/include 
                            #    "/usr/include/cpprest"
                            
                            PUBLIC
                                "libs"
                                "$<TARGET_PROPERTY:_this_ota_backend_utils,INTERFACE_INCLUDE_DIRECTORIES>"
                            INTERFACE
                                #"$<BUILD_INTERFACE:${_this_project_common_SOURCE_DIR}>"
                                "$<INSTALL_INTERFACE:include>")

##########################################################################################
#
# BLOCK-3: SRC - Сборка проекта 
# 
##########################################################################################

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/)

add_executable(${PROJECT_NAME} src/main.cpp)

# просто линкуем с либами
target_link_libraries( ${PROJECT_NAME} LINK_PRIVATE  _this_project_common )

# используем include из другого target'a
target_include_directories( ${PROJECT_NAME}
                            PUBLIC
                                "$<TARGET_PROPERTY:_this_project_common,INTERFACE_INCLUDE_DIRECTORIES>"
                            INTERFACE
                                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                                "$<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}>")

set_target_properties( ${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

##########################################################################################
#
# BLOCK-4:   Копируем ресурсы в директорию сборки
#
##########################################################################################

# Копируем в build для локального запуска
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/wrapper_apt2json.sh DESTINATION . DESTINATION ${PROJECT_SOURCE_DIR}/bin)

# Добовляем исполняемый файл в DEB
install(FILES 
            ${PROJECT_SOURCE_DIR}/bin/${PROJECT_NAME}
        DESTINATION 
            /home/user/${PROJECT_NAME}
        PERMISSIONS 
            OWNER_READ OWNER_WRITE OWNER_EXECUTE 
            GROUP_READ GROUP_EXECUTE 
            WORLD_READ WORLD_EXECUTE) #755

# Добовляем зависимые файлы в DEB
# install( FILES 
#             ${PROJECT_SOURCE_DIR}/assets/config.json
#          DESTINATION 
#             /home/user/${PROJECT_NAME})

if (${BUILD_TESTING})
    enable_testing()
    add_subdirectory(tests)
endif()

include(CMakePacking)