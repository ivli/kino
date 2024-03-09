FROM ubuntu:22.04



EXPOSE 42024

ADD ./build/kino /usr/local/bin/kino
CMD ["/usr/local/bin/kino"]
