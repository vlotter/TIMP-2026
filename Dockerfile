FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive
ENV QT_QPA_PLATFORM=offscreen

RUN apt-get update && apt-get install -y \
    build-essential \
    make \
    qt6-base-dev \
    libssl-dev \
    libqt6network6 \
    libqt6sql6 \
    libqt6sql6-sqlite \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build
COPY . .

WORKDIR /app

RUN qmake6 "CONFIG+=release" /build/timp26.pro \
    && make sub-Server -j"$(nproc)"

RUN rm -rf /build

EXPOSE 33333
CMD ["/app/Server/Server"]
