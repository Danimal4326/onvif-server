FROM debian:12 as builder
WORKDIR /build

RUN apt-get update && apt-get install -y \
    make git gcc g++ \
    gsoap libgsoap-dev libssl-dev zlib1g-dev libyaml-cpp-dev

COPY ./ /build/

RUN make

FROM debian:12-slim
RUN apt-get update && apt-get install -y \
    gsoap bash dumb-init libyaml-cpp0.7

WORKDIR /onvif-server

COPY --from=builder /build/onvif-server  onvif-server

ENTRYPOINT ["dumb-init", "--"]

CMD ["/onvif-server/onvif-server", "-c", "/onvif-server/config.yaml"]
