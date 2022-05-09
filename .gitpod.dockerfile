FROM gitpod/workspace-full

USER root

RUN install-packages libpng++-dev

WORKDIR /workspace/
RUN git clone https://github.com/catchorg/Catch2.git
WORKDIR /workspace/Catch2/
RUN git checkout v2.x
RUN cmake -Bbuild -H. -DBUILD_TESTING=OFF
RUN cmake --build build/ --target install
RUN rm -rf /workspace/Catch2

ENV PKG_CONFIG_PATH=/workspace/dependencies/libjpeg/inst/:/workspace/dependencies/libjpeg/inst/lib/pkgconfig
