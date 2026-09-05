FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    qt6-base-dev \
    qt6-base-dev-tools \
    libgl1-mesa-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . /app

# Сборка сервера в докере через qmake6
RUN mkdir build && cd build && qmake6 ../timp26.pro && make sub-Server

CMD ["./build/Server/Server"]
