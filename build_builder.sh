#!/bin/bash

# Script to build the 'builder' OCI image

PROJECT=${CI_PROJECT_NAMESPACE}/$(basename $(pwd))/builder \
CAR_OCI_REGISTRY_HOST=${CI_REGISTRY} \
DOCKER_BUILD_CONTEXT=. \
DOCKER_FILE_PATH=Dockerfile.builder \
VERSION="0.0.1" \
ADDITIONAL_ARGS="${ADDITIONAL_ARGS}" \
CI_REGISTRY="dirty hack to avoid -dev suffix on tag" \
/usr/local/bin/docker-build.sh
