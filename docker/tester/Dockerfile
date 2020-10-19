# Attempt at creating a TangoSnmp tester image
#
ARG DOCKER_REGISTRY_USER
ARG DOCKER_REGISTRY_HOST
FROM ${DOCKER_REGISTRY_HOST}/${DOCKER_REGISTRY_USER}/ska-python-buildenv:latest as buildenv

USER root

RUN chown tango:tango /app

# base image copies these in, but in this case they are useless empty files
#RUN rm Pipfile
#RUN rm Pipfile.lock

# snmp-mibs-downloader is in the non-free repo, so we add it to the sources.list
RUN cat /etc/apt/sources.list | sed 's/main$/main contrib non-free/' > /etc/apt/sources.list_new && \
    mv /etc/apt/sources.list_new /etc/apt/sources.list && \
    export DEBIAN_FRONTEND=noninteractive && apt update && \
    apt install -yq snmp-mibs-downloader && \
    apt clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*
    
# install SNMP simulator
RUN pip3 install snmpsim

USER tango
# generate an SNMP simulator configuration
#RUN mib2dev.py --mib-module=SNMPv2-MIB --start-oid=1.3.6.1.2.1.1.1 --stop-oid=1.3.6.1.2.1.1.8 --output-file=snmpv2.snmprec

# TODO this would be better as a file that we COPY in...
RUN echo "1.3.6.1.4.1.1.1.1|2:writecache|value=42" > one.snmprec


#snmpsimd.py --data-dir=. --agent-udpv4-endpoint=127.0.0.1:1024

# not sure if this command works at all
#snmpwalk -M +. -m SIMULATOR-MIB -v 2c -c one 127.0.0.1:1024 simulator.1.1

