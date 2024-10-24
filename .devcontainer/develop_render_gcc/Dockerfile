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
    cmake \
    dbus \
    doxygen \
    git \
    graphviz \
    libgl1-mesa-dev \
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
    python3 \
    sudo \
    # to be removed
    pipx && \
    # add ubuntu user to sudoers
    echo ubuntu ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/ubuntu && \
    chmod 0440 /etc/sudoers.d/ubuntu

RUN pipx install aqtinstall && \
    aqt install-qt -O /opt/qt linux desktop ${QT_VERSION}  && \
    pipx uninstall aqtinstall

# clean
RUN apt-get -y remove pipx && \
    apt-get -y autoremove && \
    apt-get -y clean && \
    rm -rf /var/lib/apt/lists/*

USER ubuntu
