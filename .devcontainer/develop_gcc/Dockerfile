FROM ubuntu:24.04

RUN apt-get -y update && DEBIAN_FRONTEND=noninteractive apt-get -y install \
    build-essential \
    catch2 \
    cmake \
    git \
    libtbb-dev \
    ninja-build \
    python3

# clean
RUN apt-get -y autoremove && \
    apt-get -y clean && \
    rm -rf /var/lib/apt/lists/*

USER ubuntu
