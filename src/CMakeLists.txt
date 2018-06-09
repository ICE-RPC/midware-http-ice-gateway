add_custom_command(
    OUTPUT "ice_interface.cpp"
    COMMAND slice2cpp --underscore ice_interface.ice
    DEPENDS ice_interface.ice
)

include_directories(
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/common/
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_SOURCE_DIR}/third-party/
    ${CMAKE_SOURCE_DIR}/third-party/evhtp/include
    ${CMAKE_SOURCE_DIR}/third-party/libcuckoo/include
    /home/apps/cpplibs/Ice-3.6.4/include
)

link_directories(
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/common/
    ${CMAKE_SOURCE_DIR}/src/
    /home/apps/cpplibs/Ice-3.6.4/lib64
    /home/apps/cpplibs/Ice-3.6.4/lib64/c++11
    ${CMAKE_SOURCE_DIR}/third-party/evhtp/lib
    ${CMAKE_SOURCE_DIR}/third-party/libcuckoo/lib
)

set(SERVER_SRC
    main.cpp
    conf.cpp
    http_handler.cpp
    ice_rqst.cpp
    ice_services.cpp
    ice_interface.cpp
    ice_resp_handler.cpp
)

ADD_EXECUTABLE(midware-http-ice-gateway ${SERVER_SRC})
target_link_libraries(midware-http-ice-gateway
    pthread
    evhtp event event_openssl event_core event_extra event_pthreads event_extra ssl crypto
    Ice IceGrid IceUtil
    cityhash
)

