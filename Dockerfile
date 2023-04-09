# Get the base Ubuntu image from Docker Hub
FROM nvidia/cuda:11.8.0-cudnn8-devel-ubuntu22.04

# Arguments for Installation procedure
ARG OPENCV_VERSION=4.5.3
ARG DEBIAN_FRONTEND="noninteractive" 

# Update apps on the base image
RUN apt-get -y update && apt-get install -y

# Install the Clang compiler
RUN apt-get -y install clang 
RUN echo 'tzdata tzdata/Areas select Europe' | debconf-set-selections
RUN echo 'tzdata tzdata/Zones/Europe select Berlin' | debconf-set-selections
RUN apt install -y tzdata
RUN apt-get -y install build-essential gdb
RUN apt-get -y install cmake
RUN apt-get -y install git
RUN apt-get -y install v4l-utils
RUN apt-get -y install libopencv-dev
RUN apt-get -y install python3-pip
RUN apt-get -y install wget unzip yasm pkg-config libswscale-dev
RUN apt-get -y install libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev
RUN apt-get -y install libavformat-dev libpq-dev libxine2-dev libglew-dev
RUN apt-get -y install libtiff5-dev zlib1g-dev libjpeg-dev libavcodec-dev libavformat-dev
RUN apt-get -y install libavutil-dev libpostproc-dev libeigen3-dev python3-dev
RUN apt-get -y install nano
RUN pip install conan==1.59.0 numpy pandas

RUN cd /opt/ &&\
    # Download and unzip OpenCV and opencv_contrib and delte zip files
    wget https://github.com/opencv/opencv/archive/$OPENCV_VERSION.zip &&\
    unzip $OPENCV_VERSION.zip &&\
    rm $OPENCV_VERSION.zip &&\
    wget https://github.com/opencv/opencv_contrib/archive/$OPENCV_VERSION.zip &&\
    unzip ${OPENCV_VERSION}.zip &&\
    rm ${OPENCV_VERSION}.zip &&\
    # Create build folder and switch to it
    mkdir /opt/opencv-${OPENCV_VERSION}/build && cd /opt/opencv-${OPENCV_VERSION}/build &&\
    # Cmake configure
    cmake \
        -DOPENCV_EXTRA_MODULES_PATH=/opt/opencv_contrib-${OPENCV_VERSION}/modules \
        -DWITH_CUDA=ON \
        -DCUDA_ARCH_BIN=7.5,8.0,8.6 \
        -DCMAKE_BUILD_TYPE=RELEASE \
        # Install path will be /usr/local/lib (lib is implicit)
        -DCMAKE_INSTALL_PREFIX=/usr/local \
        .. &&\
    # Make
    make -j"$(nproc)" && \
    # Install to /usr/local/lib
    make install && \
    ldconfig &&\
    # Remove OpenCV sources and build folder
    rm -rf /opt/opencv-${OPENCV_VERSION} && rm -rf /opt/opencv_contrib-${OPENCV_VERSION}

# Run the output program from the previous step
CMD ["bash"]

LABEL Name=FIPP_Development Version=0.0.3
