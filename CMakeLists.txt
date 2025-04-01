if(UNIX AND NOT APPLE)

    cmake_minimum_required(VERSION 3.16)
    project(mpris VERSION 5.1)

    find_package(Albert REQUIRED)

    albert_plugin(QT DBus Widgets)

    # no namespaces (class/namespace conflict)
    set_source_files_properties(mpris.xml PROPERTIES NO_NAMESPACE ON)

    # generate code into binary dir
    qt_add_dbus_interface(DBUS_SRCS mpris.xml mpris)

    target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_BINARY_DIR})

    target_sources(${PROJECT_NAME} PRIVATE mpris.xml ${DBUS_SRCS})

endif()
