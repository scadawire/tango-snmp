# Attempt at creating a TangoSnmp builder image
FROM artefact.skao.int/ska-tango-images-tango-cpp:9.3.9

USER root

# snmp-mibs-downloader is in the non-free repo, so we add it to the sources.list
RUN cat /etc/apt/sources.list | sed 's/main$/main contrib non-free/' > /etc/apt/sources.list_new && \
    mv /etc/apt/sources.list_new /etc/apt/sources.list && \
    export DEBIAN_FRONTEND=noninteractive && apt update && \
    apt install -y libsnmp-dev snmp-mibs-downloader cmake g++ && \
    apt clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

