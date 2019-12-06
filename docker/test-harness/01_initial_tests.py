import tango
import unittest

_my_server = "TangoSnmp"

def find_server():
    db = tango.Database()
    sl = db.get_server_list("{}*".format(_my_server))
    print("Found {} instances of {}".format(len(sl), _my_server))
    assert len(sl) > 0

def not_exported_device():
    not_exp = tango.DeviceProxy("not/exported/device")
    print("not_exp device state: {}".format(not_exp.state()))
    print("not_exp device status: {}".format(not_exp.status()))

def no_comms_device_state():
    no_comms = tango.DeviceProxy("no/comms/device")
    return no_comms.state()


    # TODO - refactor to use testing framework properly (Check what other SKA tango uses!)
    # - a device in its own SimulationMode (no SNMP)

class MyTestCase(unittest.TestCase):
    def test00(self):
        db = tango.Database()
        sl = db.get_server_list("{}*".format(_my_server))
        self.assertGreater(len(sl), 0, 'TangoSnmp instances in db')

    def test10(self):
        self.assertRaises(tango.ConnectionFailed, not_exported_device)
    def test11(self):
        no_comms = tango.DeviceProxy("no/comms/device")
        no_comms.init()
        self.assertEqual(no_comms.state(), tango.DevState.ON, 'no comms initial state')
        print('no comms device state after init: {}'.format(no_comms.state()))
    def test12(self):
        no_comms = tango.DeviceProxy("no/comms/device")
        with self.assertRaises(tango.DevFailed) as context:
            x = no_comms.none
        e = context.exception
        # DevFailed exception should contain two 'DevError's
        # args[0] from TangoSnmp (timeout)
        # args[1] from DeviceProxy (can't read attribute)
        self.assertEqual(e.args[0].reason, 'TangoSnmp_SnmpError', 'DevError reason 0')
        self.assertEqual(e.args[1].reason, 'API_AttributeFailed', 'DevError reason 1')
        print('no comms device state after attr fail: {}'.format(no_comms.state()))
        print('no comms device status after attr fail: {}'.format(no_comms.status()))
        # TODO - test an attribute that is not even defined in DynamicAttributes,
        # see if result is any different to above...

    def test20(self):
        snmp_sim = tango.DeviceProxy("ab/ab/simulator")
        self.assertEqual(snmp_sim.state(), tango.DevState.ON, 'SNMP sim device ON')
    def test21(self):
        snmp_sim = tango.DeviceProxy("ab/ab/simulator")
        self.assertEqual(snmp_sim.one, 42, 'SNMP sim read')
    def test22(self):
        snmp_sim = tango.DeviceProxy("ab/ab/simulator")
        snmp_sim.one = 84    
        self.assertEqual(snmp_sim.one, 84, 'SNMP sim write')

if __name__ == '__main__':
    unittest.main()
