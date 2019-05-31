FROM ubuntu:18.04
RUN apt update && apt install -y libopencv-dev
COPY main /root
WORKDIR /root

CMD ["./main"]