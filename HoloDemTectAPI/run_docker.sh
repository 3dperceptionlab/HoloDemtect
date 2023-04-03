#!/bin/bash
export containerName=FlaskAPI_$USER
sleep 3 && \
        xhost +local:`docker inspect --format='{{ .Config.Hostname }}' $containerName` >/dev/null 2>&1 &
docker run -d --rm -it \
        --volume="/home/mbenavent/workspace/HoloDemtect/HoloDemTectAPI:/workspace:rw" \
        --volume=$HOME/.Xauthority:/root/.Xauthority:ro \
        --workdir="/workspace" \
        -v $XSOCK:$XSOCK:rw \
        --net=host \
        --env="DISPLAY" \
        --name $containerName \
        python:latest bash
