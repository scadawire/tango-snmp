ARG DOCKER_REGISTRY_USER 
ARG DOCKER_REGISTRY_HOST 
FROM ${DOCKER_REGISTRY_HOST}/${DOCKER_REGISTRY_USER}/tango-cpp:latest as buildenv

USER root
RUN export DEBIAN_FRONTEND=noninteractive && apt update && \
    apt install -y libsnmp-dev libtango-dev && \
    apt clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

WORKDIR /usr/local/bin
COPY build/bin/TangoSnmp .

CMD ["TangoSnmp", "test"]
