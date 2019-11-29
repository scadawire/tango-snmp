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

WORKDIR /usr/local/bin
COPY bin/TangoSnmp .
