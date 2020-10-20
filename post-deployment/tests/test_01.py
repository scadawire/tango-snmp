import pytest
import tango
from tango import Database, DeviceProxy

@pytest.fixture(scope="class")
def no_comms():
    dev = DeviceProxy("no/comms/device")
    return dev


@pytest.fixture(scope="class")
def simulator():
    sim = DeviceProxy("ab/ab/simulator")
    return sim


class TestDb:
    def test_devices_in_db(self):
        _my_server = "TangoSnmp"
        db = tango.Database()
        sl = db.get_server_list("{}*".format(_my_server))
        assert len(sl) > 0
    def test_not_exported(self):
        not_exp = tango.DeviceProxy("not/exported/device")
        with pytest.raises(tango.ConnectionFailed):
            x = not_exp.state()

@pytest.mark.usefixtures("no_comms")
class TestNoComms:
    def test_init(self, no_comms):
        no_comms.init()
        assert no_comms.state() == tango.DevState.ON
    def test_attr_fail(self, no_comms):
        with pytest.raises(tango.DevFailed) as context:
            # this attribute is specified in the DynamicAttributes 
            x = no_comms.one
        e = context.exception
        # DevFailed exception should contain two 'DevError's
        # args[0] from TangoSnmp (timeout)
        # args[1] from DeviceProxy (can't read attribute)
        assert e.args[0].reason=='TangoSnmp_SnmpError'
        assert e.args[1].reason=='API_AttributeFailed'
        assert no_comms.state()==tango.DevState.ON
    def test_attr_not_exist(self, no_comms):
        with pytest.raises(AttributeError) as context:
            # this attribute is NOT specified in the DynamicAttributes 
            x = no_comms.attribute_that_doesnt_exist
        e = context.exception
        assert no_comms.state()==tango.DevState.ON
    def test_simulation(self):
        no_comms.GlobalSimulationEnable(True)
        # in simulation mode, returned value increments by 10 each time
        assert no_comms.one==10
        assert no_comms.one==20
        assert no_comms.one==30

@pytest.mark.usefixtures("simulator")
class TestSnmpSimulatorComms:
    def test_on(self, simulator):
        assert simulator.state()==tango.DevState.ON#, 'SNMP sim device ON')
    def test_snmp_read(self, simulator):
        # starting value of 42 is specified in one.snmprec config file
        assert simulator.one==42#, 'SNMP sim read')
    def test_simulation(self, simulator):
        # this is the Tango device simulation 
        # (i.e. it does not communicate over SNMP)
        simulator.GlobalSimulationEnable(True)
        assert simulator.one==10#, 'tango device simulation enabled'
        simulator.GlobalSimulationEnable(False)
        assert simulator.one==42#, 'tango device simulation disabled')
    def test_sim_ignores_write(self, simulator):
        #simulator.Init()
        simulator.GlobalSimulationEnable(True)
        simulator.one = 888    
        assert simulator.one!=888#, 'tango device simulation ignore write')
        simulator.GlobalSimulationEnable(False)
        assert simulator.one==42#, 'tango device simulation disabled')
    def test_snmp_write(self, simulator):
        # this write should go to the SNMP protocol simulator and then be read back
        simulator.one = 84    
        assert simulator.one==84
