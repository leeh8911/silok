FROM ubuntu:22.04

# ========================
# 기본 설정 및 필수 패키지 설치
# ========================
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    clang-format \
    clang-tidy \
    curl \
    wget \
    unzip \
    pkg-config \
    libssl-dev \
    libcrypt-dev \
    libsqlite3-dev \
    doxygen \
    graphviz \
    ca-certificates \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# ========================
# 컴파일러 설정
# ========================
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends software-properties-common && \
    add-apt-repository ppa:ubuntu-toolchain-r/test -y && \
    apt-get update && \
    apt-get install -y --no-install-recommends gcc-13 g++-13 cmake && \
    apt-get clean && rm -rf /var/lib/apt/lists/*


ENV CC=gcc-13
ENV CXX=g++-13

# ========================
# GoogleTest 설치
# ========================
RUN git clone https://github.com/google/googletest.git /opt/googletest \
    && cd /opt/googletest \
    && cmake -S . -B build \
    && cmake --build build --target install

# ========================
# spdlog 설치
# ========================
RUN git clone https://github.com/gabime/spdlog.git /opt/spdlog \
    && cd /opt/spdlog \
    && cmake -S . -B build -DSPDLOG_BUILD_EXAMPLE=OFF \
    && cmake --build build --target install

# ========================
# sqlite_orm 설치 (header-only)
# ========================
RUN mkdir -p /usr/local/include/sqlite_orm \
    && curl -L https://raw.githubusercontent.com/fnc12/sqlite_orm/master/include/sqlite_orm/sqlite_orm.h \
    -o /usr/local/include/sqlite_orm/sqlite_orm.h

# ========================
# nlohmann-json 설치 (header-only)
# ========================
RUN curl -L https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp \
    -o /usr/local/include/json.hpp

# ========================
# jwt-cpp 설치
# ========================
RUN git clone https://github.com/Thalhammer/jwt-cpp.git /opt/jwt-cpp \
    && cd /opt/jwt-cpp \
    && mkdir build && cd build \
    && cmake .. \
    && cmake --install .

# ========================
# crow 설치 (header-only)
# ========================
RUN git clone https://github.com/CrowCpp/Crow.git /opt/crow \
    && cp -r /opt/crow/include/crow /usr/local/include/

# ========================
# bcrypt 설치
# ========================
RUN git clone https://github.com/trusch/libbcrypt.git /opt/libbcrypt \
    && mkdir -p /opt/libbcrypt/build \
    && cd /opt/libbcrypt/build \
    && cmake .. \
    && make \
    && make install



# ========================
# 작업 디렉토리 설정
# ========================
WORKDIR /workspace

CMD ["/bin/bash"]
