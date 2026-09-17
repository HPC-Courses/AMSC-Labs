# Start from a basic Ubuntu 16.04 image
FROM ubuntu:16.04

# Add the instruction to install gcc here
RUN apt-get update -y && \
    apt-get install -y --no-install-recommends \
        build-essential \
        gcc && \
    rm -rf /var/lib/apt/lists/*

# Copy Hello World source code into the container image
COPY ./hello.c /var/tmp/hello.c

# Build Hello World
RUN gcc -o /usr/local/bin/hello /var/tmp/hello.c
