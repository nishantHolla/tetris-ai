#!/bin/bash

docker build --build-arg BUILD_TYPE=$1 --build-arg TEST_NAME=$2 -t tetris-ai .
