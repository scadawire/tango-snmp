ARG DOCKER_REGISTRY_USER
ARG DOCKER_REGISTRY_HOST
FROM ${DOCKER_REGISTRY_HOST}/${DOCKER_REGISTRY_USER}/tangosnmp-builder:latest as buildenv

USER root

WORKDIR /usr/local/bin
COPY build/bin/TangoSnmp .
