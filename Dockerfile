FROM ubuntu:23.04

RUN apt-get -y update && apt-get install -y \
  g++ cmake sudo gdb clang clang-tidy clang-tools libc++-dev libc++abi-dev
  
# libc++ (clang) workaround
RUN ln -s /usr/include/locale.h /usr/include/xlocale.h

# https://code.visualstudio.com/docs/devcontainers/containers
