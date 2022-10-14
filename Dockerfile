# Get the base Ubuntu image from Docker Hub
FROM ubuntu:latest

# Update apps on the base image
RUN apt-get -y update && apt-get install -y

# Install the Clang compiler
RUN apt-get -y install clang 
RUN echo 'tzdata tzdata/Areas select Europe' | debconf-set-selections
RUN echo 'tzdata tzdata/Zones/Europe select Berlin' | debconf-set-selections
RUN DEBIAN_FRONTEND="noninteractive" apt install -y tzdata
RUN apt-get -y install build-essential gdb
RUN apt-get -y install cmake
RUN apt-get -y install git
RUN apt-get -y install v4l-utils
RUN apt-get -y install libopencv-dev
RUN apt-get -Y install python3-pip
RUN pip install conan

# Run the output program from the previous step
CMD ["bash"]

LABEL Name=FIPP_Developement Version=0.0.1
