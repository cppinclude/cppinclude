FROM ubuntu

RUN apt-get update

ENV DEBIAN_FRONTEND noninteractive
RUN ln -fs /usr/share/zoneinfo/America/New_York /etc/localtime
RUN apt-get install -y gcc-10 g++-10 cmake

ADD ./ /cppinclude

WORKDIR /cppinclude/
RUN rm -rf ./build || true 

ENV CC gcc-10
ENV CXX g++-10
RUN mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
cmake --build . --config Release -j$(nproc) && \
make install && \
cd .. && \
rm -rf ./build 

WORKDIR /
