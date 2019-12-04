import tango

_my_server = "TangoSnmp"

def find_server():
    db = tango.Database()
    sl = db.get_server_list("{}*".format(_my_server))
    print("Found {} instances of {}".format(len(sl), _my_server))
    assert len(sl) > 0

def test_test_device_is_running():
    tango_test = tango.DeviceProxy("ab/ab/simulator")
    print("tango_test device state: {}".format(tango_test.state()))
    assert True

def test_snmp_simulator():
    snmp_sim = tango.DeviceProxy("ab/ab/simulator")
    print("snmp_sim attributes (pre-init):")
    for a in snmp_sim.attribute_list_query():
        print(a.name)
    print("")
    print("snmp_sim DynamicAttributes:")
    for key,val in snmp_sim.get_property('DynamicAttributes').items():    
        print(key, val)
    print("")
    print("snmp_sim state: {}".format(snmp_sim.state()))
    print("snmp_sim status: {}".format(snmp_sim.status()))
    
    print("")
    print("sending init cmd")
    snmp_sim.init()
    print("snmp_sim attributes :")
    for a in snmp_sim.attribute_list_query():
        print(a.name)
    print("")
    #  note for interactive debugging:
    #  snmp_sim.put_property({'DynamicAttributes': ['one=SIMULATOR-MIB:simulator.1.1,devlong,READ,one thing,units,%d']})
    print("snmp_sim.one: {}".format(snmp_sim.one))
    assert True

if __name__ == '__main__':
    find_server()
    test_test_device_is_running()
    test_snmp_simulator()
