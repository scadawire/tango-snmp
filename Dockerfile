ARG DOCKER_REGISTRY_USER
ARG DOCKER_REGISTRY_HOST
#FROM ${DOCKER_REGISTRY_HOST}/${DOCKER_REGISTRY_USER}/tangosnmp-builder:latest as buildenv
FROM ${DOCKER_REGISTRY_HOST}/${DOCKER_REGISTRY_USER}/ska-python-buildenv:9.3.1.1 as buildenv
FROM ${DOCKER_REGISTRY_HOST}/${DOCKER_REGISTRY_USER}/ska-python-runtime:9.3.1.1

USER root
# snmp-mibs-downloader is in the non-free repo, so we add it to the sources.list
RUN cat /etc/apt/sources.list | sed 's/main$/main contrib non-free/' > /etc/apt/sources.list_new && \
    mv /etc/apt/sources.list_new /etc/apt/sources.list && \
    export DEBIAN_FRONTEND=noninteractive && apt update && \
    apt install -y libsnmp-dev snmp-mibs-downloader && \
    apt clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*
#WORKDIR /usr/local/bin
COPY build/bin/TangoSnmp /usr/local/bin

USER tango
ENTRYPOINT ["/app/entrypoint.sh"]
CMD ["TangoSnmp", "TangoSnmp-default-instance"]
