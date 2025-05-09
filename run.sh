#!/bin/bash

docker run --name tetris-ai-container \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix tetris-ai:$1 ./tetris $@
docker cp tetris-ai-container:/app/out/tetris-ai-params.txt .
docker cp tetris-ai-container:/app/out/tetris-ai-train.log .
docker stop tetris-ai-container
docker rm tetris-ai-container
