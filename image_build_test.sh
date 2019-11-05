#!/bin/bash

DOCKER_REGISTRY_HOST=nexus.engageska-portugal.pt
DOCKER_REGISTRY_USER=ska-docker

docker build -t andrew_test:0.0.1 . --build-arg DOCKER_REGISTRY_HOST=$DOCKER_REGISTRY_HOST --build-arg DOCKER_REGISTRY_USER=$DOCKER_REGISTRY_USER


docker run -i -t andrew_test:0.0.1 /bin/bash
