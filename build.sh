#!/bin/bash

docker buildx build --load --build-arg BUILD_TYPE=$1 --build-arg TEST_NAME=$2 -t tetris-ai:$1 .
