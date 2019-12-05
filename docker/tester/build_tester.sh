#!/bin/bash

# we need our path so that docker build uses the correct Dockerfile
# (without this, if Dockerfile is in pwd, that file will be used!)
MY_PATH=$(dirname $(realpath $0))

VERSION=`date +"%Y-%m-%d"`

IMAGE=$DOCKER_REGISTRY_HOST/$DOCKER_REGISTRY_USER/tangosnmp-tester
echo building image: $IMAGE
echo version: $VERSION
echo Running build
echo 

# ska-python-buildenv wants these files to exist
# we don't need them for our purposes, 
# but if they don't exist docker build will fail
touch Pipfile
touch Pipfile.lock

docker build -t ${IMAGE}:${VERSION} $MY_PATH -f $MY_PATH/Dockerfile --build-arg DOCKER_REGISTRY_HOST=$DOCKER_REGISTRY_HOST --build-arg DOCKER_REGISTRY_USER=$DOCKER_REGISTRY_USER

if [ "$1" == "push" ]; then
    echo Pushing image
    docker push ${IMAGE}:${VERSION}
    docker tag ${IMAGE}:${VERSION} ${IMAGE}:latest
    docker push ${IMAGE}:latest
else
    echo not pushing
fi


