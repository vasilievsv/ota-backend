#
# ┌──────────────────────────────────────────────────────────────────────────────────────┐
# │                                                                                      │
# │                                CMake модуль                                          │
# │                         * * * * * * * * * * * * *                                    │
# │                               Генерация DEB                                          │
# └──────────────────────────────────────────────────────────────────────────────────────┘
#
cmake_minimum_required(VERSION 3.15)

# https://decovar.dev/blog/2021/09/23/cmake-cpack-package-deb-apt/
# https://www.mankier.com/7/cpack-generators


##########################################################################################
#
# BLOCK-0: Настройка systemd
# 
##########################################################################################
    set(VERBOSE_LOGGING         TRUE)
    set(_SYSTEMD_AUTO_ENABLE    ON )
    set(_SYSTEMD_SERVICE_NAME    ${PROJECT_NAME} )
    set(_SYSTEMD_DESCRIPTION     ${PROJECT_DESCRIPTION} )
    set(_SYSTEMD_WORK_DIR        /home/user/${PROJECT_NAME} )
    set(_SYSTEMD_EXE_PATH        /home/user/${PROJECT_NAME}/${PROJECT_NAME})
    set(_SYSTEMD_SERVICE_IN      ${CMAKE_CURRENT_LIST_DIR}/assets/debian/systemd-template.service.in)
    set(_SYSTEMD_SERVICE_OUT     ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.service)
# шаблонизатор
configure_file(${_SYSTEMD_SERVICE_IN} ${_SYSTEMD_SERVICE_OUT} @ONLY  IMMEDIATE)
install(FILES  ${_SYSTEMD_SERVICE_OUT} DESTINATION /usr/local/share/ )


##########################################################################################
#
# BLOCK-1: Описание DEB пакета
# 
##########################################################################################

#set(CPACK_PACKAGE_VERSION               ${${PROJECT_NAME}_VERSION})
#set(CPACK_PACKAGE_VERSION_MAJOR         ${PROJECT_VERSION_MAJOR})
#set(CPACK_PACKAGE_VERSION_MINOR         ${PROJECT_VERSION_MINOR})
#set(CPACK_PACKAGE_VERSION_PATCH         ${PROJECT_VERSION_PATCH})

set(CPACK_PACKAGE_NAME                  ${PROJECT_NAME}                   CACHE STRING "The package name")
set(CPACK_PACKAGE_VENDOR                "Public")
set(CPACK_PACKAGE_CONTACT               "YOUR@E-MAIL.net")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY   ${${PROJECT_NAME}_DESCRIPTION}    CACHE STRING "Package description")
set(CPACK_PACKAGE_INSTALL_DIRECTORY     ${CPACK_PACKAGE_NAME})

set(CPACK_VERBATIM_VARIABLES            YES)
set(CPACK_STRIP_FILES                   YES)
set(CPACK_SET_DESTDIR                   YES)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS      NO)
set(CPACK_GENERATOR                     "DEB")
set(CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE 1)

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README  "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

set(CPACK_COMPONENTS_GROUPING   ALL_COMPONENTS_IN_ONE)#ONE_PER_GROUP)

set(CPACK_OUTPUT_FILE_PREFIX        "${CMAKE_CURRENT_SOURCE_DIR}/bin")

##########################################################################################
#
# BLOCK-2: Параметризация Post cкриптов
# 
##########################################################################################

set(VERBOSE_LOGGING FALSE)
    set(POST_IN     ${CMAKE_CURRENT_LIST_DIR}/assets/debian/postinst.in)
    set(POST_OUT    ${CMAKE_CURRENT_BINARY_DIR}/postinst)
configure_file(${POST_IN} ${POST_OUT} @ONLY IMMEDIATE)

    set(POSTUN_IN   ${CMAKE_CURRENT_LIST_DIR}/assets/debian/postrm.in)
    set(POSTUN_OUT  ${CMAKE_CURRENT_BINARY_DIR}/postrm)
configure_file(${POSTUN_IN} ${POSTUN_OUT} @ONLY IMMEDIATE)

    set(PREUN_IN    ${CMAKE_CURRENT_LIST_DIR}/assets/debian/prerm.in)
    set(PREUN_OUT   ${CMAKE_CURRENT_BINARY_DIR}/prerm)
configure_file(${PREUN_IN} ${PREUN_OUT} @ONLY IMMEDIATE)

set(CPACK_DEB_POST_INSTALL_SCRIPT_FILE      ${POST_OUT})
set(CPACK_DEB_POST_UNINSTALL_SCRIPT_FILE    ${POSTUN_OUT})
set(CPACK_DEB_PRE_UNINSTALL_SCRIPT_FILE     ${PREUN_OUT})
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA      ${POSTUN_OUT}  ${POST_OUT} ${PREUN_OUT})

set(CPACK_DEBIAN_FILE_NAME              DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER     "YOUR NAME")
set(CPACK_DEB_PACKAGE_REQUIRES_POSTUN   "systemd")
set(CPACK_DEB_PACKAGE_REQUIRES_PREUN    "systemd")
set(CPACK_DEB_PACKAGE_REQUIRES_POST     "systemd")
set(CPACK_PACKAGING_INSTALL_PREFIX      ${CMAKE_INSTALL_PREFIX})

set(
    CPACK_INSTALL_DEFAULT_DIRECTORY_PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)

set(CPACK_ARCHIVE_COMPONENT_INSTALL 0)

include(CPack)
