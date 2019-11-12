import tango

_my_server = "TangoSnmp"

def find_server():
    db = tango.Database()
    sl = db.get_server_list("{}*".format(_my_server))
    print("Found {} instances of {}".format(sl, _my_server))
    assert len(sl) > 0

def test_test_device_is_running():
    tango_test = tango.DeviceProxy("ab/ab/48v")
    # no connection to this device, should be in FAULT state
    assert tango_test.state() == tango.DevState.FAULT


if __name__ == '__main__':
    find_server()
    test_test_device_is_running()
