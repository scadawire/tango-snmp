add_executable (TangoSnmp TangoSnmp.cpp TangoSnmpClass.cpp TangoSnmpStateMachine.cpp TangoSnmpDynAttrUtils.cpp ClassFactory.cpp main.cpp)
target_include_directories(TangoSnmp PRIVATE .)
#target_include_directories(TangoSnmp PUBLIC /usr/include/tango)
target_include_directories(TangoSnmp PUBLIC ${TANGO_INCLUDE_DIRS})
message("tango include dir: ${TANGO_INCLUDE_DIRS}")

target_link_libraries(TangoSnmp netsnmp m tango omniDynamic4 COS4 omniORB4 omnithread log4tango zmq dl pthread stdc++)

#set_target_properties(TangoSnmp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Note: old Makefile used `net-snmp-config --libs` and `net-snmp-config --cflags` to get correct snmp settings.
# I don't know how to do that kind of thing in cmake...
