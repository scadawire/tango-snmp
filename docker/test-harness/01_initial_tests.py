import tango
import unittest

    # TODO - refactor to use testing framework properly (Check what other SKA tango uses!)

class MyTestCase(unittest.TestCase):
    def test00(self):
        _my_server = "TangoSnmp"
        db = tango.Database()
        sl = db.get_server_list("{}*".format(_my_server))
        self.assertGreater(len(sl), 0, 'TangoSnmp instances in db')

    def test10(self):
        not_exp = tango.DeviceProxy("not/exported/device")
        with self.assertRaises(tango.ConnectionFailed) as context:
            x = not_exp.state()
    def test11(self):
        no_comms = tango.DeviceProxy("no/comms/device")
        no_comms.init()
        self.assertEqual(no_comms.state(), tango.DevState.ON, 'no comms initial state')
    def test12(self):
        no_comms = tango.DeviceProxy("no/comms/device")
        with self.assertRaises(tango.DevFailed) as context:
            # this attribute is specified in the DynamicAttributes 
            x = no_comms.one
        e = context.exception
        # DevFailed exception should contain two 'DevError's
        # args[0] from TangoSnmp (timeout)
        # args[1] from DeviceProxy (can't read attribute)
        self.assertEqual(e.args[0].reason, 'TangoSnmp_SnmpError', 'DevError reason 0')
        self.assertEqual(e.args[1].reason, 'API_AttributeFailed', 'DevError reason 1')
        self.assertEqual(no_comms.state(), tango.DevState.ON, 'no comms state after SNMP error')
    def test13(self):
        no_comms = tango.DeviceProxy("no/comms/device")
        with self.assertRaises(AttributeError) as context:
            # this attribute is NOT specified in the DynamicAttributes 
            x = no_comms.attribute_that_doesnt_exist
        e = context.exception
        self.assertEqual(no_comms.state(), tango.DevState.ON, 'no comms state after AttributeError')
    def test14(self):
        no_comms = tango.DeviceProxy("no/comms/device")
        no_comms.GlobalSimulationEnable(True)
        # in simulation mode, returned value increments by 10 each time
        self.assertEqual(no_comms.one, 10)
        self.assertEqual(no_comms.one, 20)
        self.assertEqual(no_comms.one, 30)

    def test20(self):
        snmp_sim = tango.DeviceProxy("ab/ab/simulator")
        self.assertEqual(snmp_sim.state(), tango.DevState.ON, 'SNMP sim device ON')
    def test21(self):
        snmp_sim = tango.DeviceProxy("ab/ab/simulator")
        # starting value of 42 is specified in one.snmprec config file
        self.assertEqual(snmp_sim.one, 42, 'SNMP sim read')
    def test22(self):
        snmp_sim = tango.DeviceProxy("ab/ab/simulator")
        snmp_sim.GlobalSimulationEnable(True)
        self.assertEqual(snmp_sim.one, 10, 'tango device simulation enabled')
        snmp_sim.GlobalSimulationEnable(False)
        self.assertEqual(snmp_sim.one, 42, 'tango device simulation disabled')
    def test23(self):
        snmp_sim = tango.DeviceProxy("ab/ab/simulator")
        #snmp_sim.Init()
        snmp_sim.GlobalSimulationEnable(True)
        snmp_sim.one = 888    
        self.assertNotEqual(snmp_sim.one, 888, 'tango device simulation ignore write')
        snmp_sim.GlobalSimulationEnable(False)
        self.assertEqual(snmp_sim.one, 42, 'tango device simulation disabled')
    def test24(self):
        snmp_sim = tango.DeviceProxy("ab/ab/simulator")
        snmp_sim.one = 84    
        self.assertEqual(snmp_sim.one, 84, 'SNMP sim write')

if __name__ == '__main__':
    unittest.main()
