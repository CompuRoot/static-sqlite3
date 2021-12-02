FROM alpine:latest

LABEL org.sqlite.static.binary.linux.x86_64.dockerfile.version="1.0.6"

ARG URL_SQLITE_SOURCE_ZIP
ARG COMPRESS_SQLITE3

# Version of SQLite to download (full url to sqlite-amalgamation.zip)
ENV SQLITE_URL_SRC="${URL_SQLITE_SOURCE_ZIP}"
ENV COMPRESSOR="${COMPRESS_SQLITE3}"

RUN echo >> /etc/apk/repositories                                       &&  \
    echo "http://dl-cdn.alpinelinux.org/alpine/edge/community"              \
          >> /etc/apk/repositories                                      &&  \
    apk update && apk upgrade                                           &&  \
    apk add --no-cache fakeroot musl-dev linux-headers gcc libgcc ccache    \
            binutils upx file wget unzip zlib-dev zlib-static               \
            libhistory readline-static readline-dev                         \
            ncurses-static ncurses-dev

WORKDIR /app

COPY Build.sh /app/
RUN  chmod +x /app/Build.sh && ./Build.sh "${SQLITE_URL_SRC}" "${COMPRESSOR}"

