#ARG DOCKER_REGISTRY_USER
#ARG DOCKER_REGISTRY_HOST
###FROM ${DOCKER_REGISTRY_HOST}/${DOCKER_REGISTRY_USER}/tangosnmp-builder:latest as buildenv
#FROM ${DOCKER_REGISTRY_HOST}/${DOCKER_REGISTRY_USER}/ska-python-buildenv:9.3.1.1 as buildenv
#FROM ${DOCKER_REGISTRY_HOST}/${DOCKER_REGISTRY_USER}/ska-python-runtime:9.3.1.1
FROM artefact.skao.int/ska-tango-images-pytango-builder:9.3.10 as buildenv
FROM artefact.skao.int/ska-tango-images-pytango-runtime:9.3.10


USER root
# snmp-mibs-downloader is in the non-free repo, so we add it to the sources.list
RUN cat /etc/apt/sources.list | sed 's/main$/main contrib non-free/' > /etc/apt/sources.list_new && \
    mv /etc/apt/sources.list_new /etc/apt/sources.list && \
    export DEBIAN_FRONTEND=noninteractive && apt update && \
    apt install -y libsnmp-dev snmp-mibs-downloader && \
    apt clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*
COPY build/bin/TangoSnmp /usr/local/bin

# this should install snmpsim, for use in testing
RUN pip install -e .

USER tango
ENTRYPOINT ["/app/entrypoint.sh"]
CMD ["TangoSnmp", "TangoSnmp-default-instance"]
