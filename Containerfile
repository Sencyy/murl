FROM gcc:14.3 as build

COPY . /murl

WORKDIR /murl
RUN ./build.sh
RUN strip build/bin/murl

FROM debian:trixie-slim

COPY --from=build /murl/build /murl

WORKDIR /murl

ENV MURL_DICT_PATH=/murl/contrib/dict
ENV MURL_FRONTEND_PATH=/murl/frontend/index.html
ENV MURL_SHORTIES_PATH=/murl/data/shorties

EXPOSE 6969
CMD [ "/murl/bin/murl" ]
