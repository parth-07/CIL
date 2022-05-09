FROM gitpod/workspace-full

USER root

RUN install-packages libpng++-dev

RUN mkdir -p /workspace/dependencies/libjpeg/src \
             /workspace/dependencies/libjpeg/inst
WORKDIR /workspace/dependencies/libjpeg
RUN wget -O libjpeg.tar.gz https://www.ijg.org/files/jpegsrc.v9e.tar.gz
RUN tar -xf libjpeg.tar.gz -C src/ --strip-components=1
WORKDIR /workspace/dependencies/libjpeg/src
RUN ./configure --prefix=`pwd`/../inst
RUN make install

WORKDIR /workspace/dependencies
RUN git clone https://github.com/catchorg/Catch2.git
WORKDIR /workspace/dependencies/Catch2/
RUN git checkout v2.x
RUN cmake -Bbuild -H. -DBUILD_TESTING=OFF
RUN cmake --build build/ --target install

ENV PKG_CONFIG_PATH=/workspace/dependencies/libjpeg/inst/:/workspace/dependencies/libjpeg/inst/lib/pkgconfig\