# BUILD:  docker build -t docker-bourne .
# RUN:    docker run --user kuba -it --security-opt seccomp=unconfined -v /Users/kuba/kuba/type_list:/home/kuba/src -v /Users/kuba/type_list_build:/home/kuba/build docker-kuba
# WINDOWS: winpty docker run --user kuba -it -v //c//Users//User//bourne:/home/kuba/src docker-bourne

FROM ubuntu:23.04

RUN apt-get -y update && apt-get install -y \
  g++ cmake sudo gdb clang clang-tidy clang-tools libc++-dev libc++abi-dev
  
RUN sudo useradd -m kuba 
RUN echo "kuba:kuba" | chpasswd 

# libc++ (clang) workaround
RUN ln -s /usr/include/locale.h /usr/include/xlocale.h

# sudo without a password
RUN echo 'kuba ALL=(ALL) NOPASSWD: ALL' >> /etc/sudoers

WORKDIR /home/kuba/build
RUN chown -R kuba:kuba /home/kuba

USER kuba


# https://code.visualstudio.com/docs/devcontainers/containers
