cmake_minimum_required(VERSION 3.16)
project(mediaremote VERSION 5.1)

find_package(Albert REQUIRED)

albert_plugin(
    SOURCES
        src/*.h src/*.cpp include/albert/plugin/*.h
    INCLUDE
        INTERFACE include
        PRIVATE src/ include/albert/plugin
    QT Widgets
)

if(APPLE)

    target_compile_options(${PROJECT_NAME} PRIVATE
        "-fobjc-arc")

    target_link_libraries(${PROJECT_NAME} PRIVATE
        "objc"
        "-framework Foundation"
        "-F/System/Library/PrivateFrameworks"
        "-framework MediaRemote"
    )

    file(GLOB PLATFORM_SOURCES
        src/mediaremote/*.h
        src/mediaremote/*.mm)

    target_sources(${PROJECT_NAME} PRIVATE ${PLATFORM_SOURCES})

elseif(UNIX)

    # find_package(Qt6 REQUIRED COMPONENTS DBus)
    # target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::DBus)

    # target_include_directories(${PROJECT_NAME} PRIVATE src/bluez ${PROJECT_BINARY_DIR})
    # set(DBUS_XML src/bluez/bluez.xml)
    # set_source_files_properties(${DBUS_XML} PROPERTIES NO_NAMESPACE ON)
    # set_source_files_properties(${DBUS_XML} PROPERTIES INCLUDE custom_types.h)
    # qt_add_dbus_interface(DBUS_SRCS ${DBUS_XML} bluez)

    # file(GLOB PLATFORM_SOURCES
    #     src/bluez/*.h
    #     src/bluez/*.cpp
    # )
    # target_sources(${PROJECT_NAME} PRIVATE
    #     ${PLATFORM_SOURCES}
    #     ${PROJECT_BINARY_DIR}/bluez.h
    #     ${DBUS_XML}
    #     ${DBUS_SRCS}
    # )

endif()
