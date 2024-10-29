FROM ubuntu:24.04

ARG QT_VERSION=6.7.2

ENV LANG C.UTF-8
ENV LC_ALL C.UTF-8

ENV PATH=/opt/qt/${QT_VERSION}/gcc_64/bin:/root/.local/bin:$PATH
ENV LD_LIBRARY_PATH=/opt/qt/${QT_VERSION}/gcc_64/lib/
ENV QT_PLUGIN_PATH=/opt/qt/${QT_VERSION}/gcc_64/plugins/
ENV QML_IMPORT_PATH=/opt/qt/${QT_VERSION}/gcc_64/qml/
ENV QML2_IMPORT_PATH=/opt/qt/${QT_VERSION}/gcc_64/qml/

RUN apt-get -y update && DEBIAN_FRONTEND=noninteractive apt-get -y install \
    build-essential \
    catch2 \
    clangd-18 \
    cmake \
    dbus \
    doxygen \
    git \
    graphviz \
    libeigen3-dev \
    libgl1-mesa-dev \
    libglfw3-dev \
    libglu1-mesa-dev \
    libtbb-dev \
    libx11-dev \
    libxi-dev \
    libxinerama-dev \
    libxcb-cursor0 \
    libxkbcommon-dev \
    libxcursor-dev \
    libxrandr-dev \
    ninja-build \
    pkg-config \
    python3 \
    sudo \
    # to be removed
    pipx && \
    # add ubuntu user to sudoers
    echo ubuntu ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/ubuntu && \
    chmod 0440 /etc/sudoers.d/ubuntu

# qt
RUN pipx install aqtinstall && \
    aqt install-qt -O /opt/qt linux desktop ${QT_VERSION}  && \
    pipx uninstall aqtinstall

# bgfx
RUN mkdir -p ~/bgfx && \
    cd ~/bgfx && \
    git clone --recursive https://github.com/bkaradzic/bgfx.cmake && \
    cd bgfx.cmake && \
    mkdir build && cd build && \
    cmake -GNinja -DCMAKE_BUILD_TYPE=Release -DBGFX_LIBRARY_TYPE=SHARED -DCMAKE_INSTALL_PREFIX=/usr .. && \
    ninja && \
    ninja install && \
    rm -rf ~/bgfx

# clean
RUN apt-get -y remove pipx && \
    apt-get -y autoremove && \
    apt-get -y clean && \
    rm -rf /var/lib/apt/lists/*

USER ubuntu