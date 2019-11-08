# 
########
# this is the CSP LMC Dockerfile
########
#FROM nexus.engageska-portugal.pt/ska-docker/ska-python-buildenv:0.2.2 AS buildenv
#FROM nexus.engageska-portugal.pt/ska-docker/ska-python-runtime:0.2.2 AS runtime
#
# create ipython profile to so that itango doesn't fail if ipython hasn't run yet
#RUN ipython profile create
#install lmc-base-classes
#USER root
#RUN DEBIAN_FRONTEND=noninteractive pip3 install https://nexus.engageska-portugal.pt/repository/pypi/packages/lmcbaseclasses/0.1.3+163bf057/lmcbaseclasses-0.1.3+163bf057.tar.gz
#
#CMD ["/venv/bin/python", "/app/csplmc/CspMaster/CspMaster.py"]


########
# this is my hack
########

ARG DOCKER_REGISTRY_USER
ARG DOCKER_REGISTRY_HOST
FROM ${DOCKER_REGISTRY_HOST}/${DOCKER_REGISTRY_USER}/tango-cpp:latest as buildenv

USER root

# snmp-mibs-downloader is in the non-free repo, so we add it to the sources.list
RUN cat /etc/apt/sources.list | sed 's/main$/main contrib non-free/' > /etc/apt/sources.list_new && \
    mv /etc/apt/sources.list_new /etc/apt/sources.list && \
    export DEBIAN_FRONTEND=noninteractive && apt update && \
    apt install -y libsnmp-dev snmp-mibs-downloader && \
    apt clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

WORKDIR /root
COPY mibs .snmp/mibs

WORKDIR /usr/local/bin
COPY bin/TangoSnmp .

#CMD ["TangoSnmp", "andrew"]

