FROM ubuntu:18.04
ARG EOS=2.0.9
ARG CDT=1.7.0
USER root
WORKDIR /nodeos
RUN apt-get update  \
    && ln -fs /usr/share/zoneinfo/Etc/UTC /etc/localtime \
    && export DEBIAN_FRONTEND=noninteractive \
    && apt-get install -y gnupg jq vim curl wget unzip git make cmake build-essential python sudo tzdata \
    && dpkg-reconfigure -f noninteractive tzdata \
    && wget -nv https://github.com/EOSIO/eos/releases/download/v${EOS}/eosio_${EOS}-1-ubuntu-18.04_amd64.deb \
    && wget -nv https://github.com/EOSIO/eosio.cdt/releases/download/v${CDT}/eosio.cdt_${CDT}-1-ubuntu-18.04_amd64.deb \
    && apt install -y ./eosio_${EOS}-1-ubuntu-18.04_amd64.deb ./eosio.cdt_${CDT}-1-ubuntu-18.04_amd64.deb \
    && rm -f ./eosio_${EOS}-1-ubuntu-18.04_amd64.deb ./eosio.cdt_${CDT}-1-ubuntu-18.04_amd64.deb \
    && rm -rf "/var/lib/apt/lists/*"
CMD nodeos -e \
-p eosio \
-p eosproducera \
-p eosproducerb \
-p eosproducerc \
-p eosproducerd \
-p eosproducere \
-p eosproducerf \
-p eosproducerg \
-p eosproducerh \
-p eosproduceri \
-p eosproducerj \
-p eosproducerk \
-p eosproducerl \
-p eosproducerm \
-p eosproducern \
-p eosproducero \
-p eosproducerp \
-p eosproducerq \
-p eosproducerr \
-p eosproducers \
-p eosproducert \
-p eosproduceru \
--plugin eosio::net_api_plugin \
--plugin eosio::net_plugin \
--plugin eosio::producer_plugin \
--plugin eosio::producer_api_plugin \
--plugin eosio::chain_api_plugin \
--plugin eosio::history_plugin \
--plugin eosio::history_api_plugin \
--plugin eosio::http_plugin \
--http-server-address=0.0.0.0:8888 \
--access-control-allow-origin=* \
--contracts-console \
--http-validate-host=false \
--trace-history \
--filter-on=* \
--verbose-http-errors \
--max-transaction-time=500000 \
--abi-serializer-max-time-ms=999915000 \
--http-max-response-time-ms=50000 \
--disable-replay-opts
EXPOSE 8888
