ARG DOCKER_REGISTRY_USER
ARG DOCKER_REGISTRY_HOST
#FROM ${DOCKER_REGISTRY_HOST}/${DOCKER_REGISTRY_USER}/tangosnmp-builder:latest as buildenv
FROM ${DOCKER_REGISTRY_HOST}/${DOCKER_REGISTRY_USER}/ska-python-buildenv:9.3.1.1 as buildenv
FROM ${DOCKER_REGISTRY_HOST}/${DOCKER_REGISTRY_USER}/ska-python-runtime:9.3.1.1

USER root
#WORKDIR /usr/local/bin
COPY build/bin/TangoSnmp /usr/local/bin

USER tango
ENTRYPOINT ["/app/entrypoint.sh"]
CMD ["TangoSnmp", "TangoSnmp-default-instance"]
