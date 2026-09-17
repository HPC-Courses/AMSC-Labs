# The "build" stage of the multi-stage Dockerfile

# Start from a basic Ubuntu 16.04 image
FROM ubuntu:16.04 AS build

RUN apt-get update -y && \
    apt-get install -y --no-install-recommends \
        build-essential \
        gcc && \
    rm -rf /var/lib/apt/lists/*

# Copy Hello World source code into the build stage
COPY ./hello.c /var/tmp/hello.c

# Build Hello World
RUN gcc -o /usr/local/bin/hello /var/tmp/hello.c

# The "runtime" stage of the multi-stage Dockerfile
# This starts an entirely new container image

# Start from a basic Ubuntu 16.04 image
FROM ubuntu:16.04

# Copy the hello binary from the build stage
COPY --from=build /usr/local/bin/hello /usr/local/bin/hello
