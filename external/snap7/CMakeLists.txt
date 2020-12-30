cmake_minimum_required(VERSION 3.16)

project(Snap7)


set(SRCS_CORE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/s7_client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/s7_client.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/s7_firmware.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/s7_isotcp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/s7_isotcp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/s7_micro_client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/s7_micro_client.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/s7_partner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/s7_partner.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/s7_peer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/s7_peer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/s7_server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/s7_server.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/s7_text.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/s7_text.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/s7_types.h
)

set(SRCS_SYS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sys/snap_msgsock.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sys/snap_msgsock.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sys/snap_platform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sys/snap_sysutils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sys/snap_sysutils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sys/snap_tcpsrvr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sys/snap_tcpsrvr.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sys/snap_threads.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sys/snap_threads.h
)

if(WIN32)
    set(SRCS_SYS ${SRCS_SYS} ${CMAKE_CURRENT_SOURCE_DIR}/src/sys/win_threads.h)
else()
    set(SRCS_SYS ${SRCS_SYS} ${CMAKE_CURRENT_SOURCE_DIR}/src/sys/unix_threads.h)
endif()

set (SRCS_LIB
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/snap7_libmain.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/snap7_libmain.cpp
)

set(SRCS_WRAPPER
    ${Snap7_SOURCE_DIR}/release/Wrappers/c-cpp/snap7.h
    ${Snap7_SOURCE_DIR}/release/Wrappers/c-cpp/snap7.cpp
)



add_library(
    ${PROJECT_NAME}
    STATIC
        ${SRCS_CORE}
        ${SRCS_SYS}
        ${SRCS_LIB}
        ${SRCS_WRAPPER}
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/release/Wrappers/c-cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/src/sys
)

if(WIN32)
    target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
            ws2_32.lib
            winmm.lib
        )
elseif(UNIX)
    target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
            pthread
    )
endif()
