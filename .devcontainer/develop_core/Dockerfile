FROM ubuntu:24.04

RUN apt-get -y update && DEBIAN_FRONTEND=noninteractive apt-get -y install \
    build-essential \
    catch2 \
    clang-18 \
    clang-tidy-18 \
    clangd-18 \
    cmake \
    doxygen \
    git \
    graphviz \
    libeigen3-dev \
    libtbb-dev \
    ninja-build \
    python3 \
    sudo && \
    # add ubuntu user to sudoers
    echo ubuntu ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/ubuntu && \
    chmod 0440 /etc/sudoers.d/ubuntu

# clean
RUN apt-get -y autoremove && \
    apt-get -y clean && \
    rm -rf /var/lib/apt/lists/*

USER ubuntu
