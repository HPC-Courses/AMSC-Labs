FROM ubuntu:24.04
MAINTAINER paolojoseph.baioni@polimi.it

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update -y && \
    apt-get install -y --no-install-recommends \
        apt-utils \
        build-essential \
        bash-completion \
        ca-certificates \
        clang \
        clang-format \
        clang-tidy \
        clang-tools \
        cmake \
        cppcheck \
        curl \
        doxygen \
        emacs-nox \
        gfortran \
        git \
        gnuplot \
        graphviz \
        less \
        libboost-all-dev \
        libblas-dev \
        libeigen3-dev \
        libhdf5-serial-dev \
        liblapack-dev \
        liblapacke-dev \
        libopenblas-dev \
        libopenmpi-dev \
        libtbb-dev \
        libsuitesparse-dev \
        libgnuplot-iostream-dev \
        nano \
        net-tools \
        octave \
        openmpi-bin \
        python-is-python3 \
        python3 \
        python3-pip \
        python3-venv \
        procps \
        r-base \
        sudo \
        software-properties-common \
        openssh-client \
        tar \
        tzdata \
        unzip \
        vim \
        wget \
        zip \
        zsh && \
    apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/*

ENV mkSuitesparseInc=/usr/include/suitesparse/
ENV mkOpenblasInc=/usr/include/x86_64-linux-gnu/openblas-pthread
ENV mkTbbInc=/usr/include
ENV mkHdf5Lib=/usr/lib/x86_64-linux-gnu/hdf5/serial/
ENV mkHdf5Inc=/usr/include/hdf5/serial/
ENV mkEigenInc=/usr/include/eigen3
ENV mkTbbLib=/usr/lib/x86_64-linux-gnu/
ENV mkEigenHome=/usr/include/eigen3
ENV mkBoostLib=/usr/lib/x86_64-linux-gnu/
#ENV mkPybind11Prefix=${PACS_ROOT}/share/cmake/
ENV mkCxxCompiler=/usr/bin/g++
ENV mkOpenblasLib=/usr/lib/x86_64-linux-gnu/openblas-pthread/
ENV mkSuitesparseLib=/usr/lib/x86_64-linux-gnu
ENV mkBoostInc=/usr/include

RUN wget https://github.com/pcafrica/mk/releases/download/v2024.0/mk-2024.0-full.tar.gz && \
    tar xvzf mk-2024.0-full.tar.gz -C / && rm mk-2024.0-full.tar.gz

RUN usermod -aG sudo ubuntu && \
    echo "ubuntu ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/ubuntu && \
    chmod 0440 /etc/sudoers.d/ubuntu

USER ubuntu
WORKDIR /home/ubuntu
