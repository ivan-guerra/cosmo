#!/bin/bash

# This script launches a docker container that is ready to build and run cosmo.
# The docker container mounts the project directory as a volume under /cosmo.
# The idea here is that you can edit files on a host PC in your IDE of
# choice. When you are ready to build/run, execute this launch script and
# build/run cosmo from within the container. No need to install a cross
# compiler, emulator, etc., it's all handled by the container.

# Source the project configuration.
source config_cosmo.sh

# Use the latest cosmo development container. If you don't want to pull
# from the repo, build the latest image using the Dockerfile at the project
# root and then update COSMO_IMAGE to point to the new image.
COSMO_IMAGE="iguerra130154/cosmo:latest"

# Credit to this SO post that shows a method for generating an Xauthority file
# on the fly.
# https://stackoverflow.com/questions/16296753/can-you-run-gui-applications-in-a-linux-docker-container/25280523#25280523
XSOCK="/tmp/.X11-unix"
XAUTH="/tmp/.docker.xauth"
touch ${XAUTH}
xauth nlist $DISPLAY | sed -e 's/^..../ffff/' | xauth -f $XAUTH nmerge -

docker run --rm -it                      \
    -v ${XSOCK}:${XSOCK}                 \
    -v ${XAUTH}:${XAUTH}                 \
    -e XAUTHORITY=${XAUTH}               \
    -e DISPLAY=${DISPLAY}                \
    -u $(id -u ${USER}):$(id -g ${USER}) \
    -v "${COSMO_PROJECT_PATH}":/cosmo    \
    ${COSMO_IMAGE}
