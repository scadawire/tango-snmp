#!/usr/bin/env bash
# Launch script for TangoSnmp Device and SNMP simulator
#
# To run Tango device, use "TangoSnmp" and supply instance argument
#  (and optionally, any further arguments)
#
# To run Tango device with SNMP simulator in same container, 
#  set the SNMP_SIMULATOR variable to non-zero

# this is a nasty hack, should be mounted as a volume or something.
mkdir -p /home/tango/.snmp && ln -s /app/charts/tangosnmp/data/mibs /home/tango/.snmp/mibs

if [ "$1" = 'TangoSnmp' ]; then
    if [ -n "$SNMP_SIMULATOR" ] && [ "$SNMP_SIMULATOR" -ne 0 ]; then
        snmpsimd.py --data-dir=. --agent-udpv4-endpoint=127.0.0.1:1024 &
    fi
    
    if [ -d /home/tango/.snmp/mibs ]; then 
        echo 'found mibs'
        ls -laH /home/tango/.snmp/mibs
    else 
        echo 'no mibs!'
    fi

    wait-for-it.sh ${TANGO_HOST} --timeout=30 --strict --
        TangoSnmp "${@:2}"
    exit $?
fi

# otherwise, just run the command as-is
exec "$@"
