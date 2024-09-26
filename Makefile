SYSROOT?=$(shell $(CXX) --print-sysroot)
PREFIX?=/usr
DESTDIR?=$(PREFIX)/bin
$(info PREFIX=$(PREFIX) SYSROOT=$(SYSROOT) DESTDIR=$(DESTDIR))

VERSION=$(shell git describe --tags --always --dirty)
$(info VERSION=$(VERSION))

GSOAP_PREFIX?=$(SYSROOT)/usr
GSOAP_BIN?=$(GSOAP_PREFIX)/bin
GSOAP_BASE=$(GSOAP_PREFIX)/share/gsoap
GSOAP_PLUGINS=$(GSOAP_BASE)/plugin

CXXFLAGS+=-std=c++11 -g2 -I inc -I ws-discovery/gsoap/ -Wunreachable-code -Wunused -Wno-deprecated-declarations
CXXFLAGS+=-I gen -I $(GSOAP_PREFIX)/include -I $(GSOAP_PLUGINS)
CXXFLAGS+=-DWITH_OPENSSL -DSOAP_PURE_VIRTUAL -fpermissive -pthread -DVERSION=\"$(VERSION)\"
LDFLAGS+=-L $(GSOAP_PREFIX)/lib/ -lgsoapssl++ -lz -pthread -lssl -lcrypto -ldl -lyaml-cpp -static-libstdc++

WSSE_SRC=$(GSOAP_PLUGINS)/wsseapi.c $(GSOAP_PLUGINS)/smdevp.c $(GSOAP_PLUGINS)/mecevp.c $(GSOAP_BASE)/custom/struct_timeval.c
WSSE_OBJ=$(WSSE_SRC:%.c=%.o)

SOAP_SRC=$(wildcard gen/soapC_*.cpp)
SOAP_OBJ=$(SOAP_SRC:%.cpp=%.o)

SERVER_OBJ=gen/soapDeviceBindingService.o gen/soapDeviceIOBindingService.o
SERVER_OBJ+=gen/soapMediaBindingService.o gen/soapImagingBindingService.o 
SERVER_OBJ+=gen/soapPTZBindingService.o
SERVER_OBJ+=gen/soapEventBindingService.o gen/soapPullPointSubscriptionBindingService.o gen/soapNotificationProducerBindingService.o gen/soapSubscriptionManagerBindingService.o
SERVER_OBJ+=gen/soapRecordingBindingService.o gen/soapReplayBindingService.o gen/soapSearchBindingService.o gen/soapReceiverBindingService.o 
SERVER_OBJ+=gen/soapDisplayBindingService.o

CLIENT_OBJ=gen/soapDeviceBindingProxy.o gen/soapDeviceIOBindingProxy.o
CLIENT_OBJ+=gen/soapMediaBindingProxy.o gen/soapImagingBindingProxy.o
CLIENT_OBJ+=gen/soapPTZBindingProxy.o
CLIENT_OBJ+=gen/soapEventBindingProxy.o gen/soapPullPointSubscriptionBindingProxy.o gen/soapNotificationProducerBindingProxy.o gen/soapSubscriptionManagerBindingProxy.o
CLIENT_OBJ+=gen/soapRecordingBindingProxy.o gen/soapReplayBindingProxy.o gen/soapReceiverBindingProxy.o gen/soapSearchBindingProxy.o 
CLIENT_OBJ+=gen/soapDisplayBindingProxy.o

all: gen/onvif.h libwsdd.a onvif-server 

gen/onvif.h: $(wildcard wsdl/*) | libwsdd.a
	mkdir -p gen
	$(GSOAP_BIN)/wsdl2h -d -Ntev -W -L -o $@ $^
	$(GSOAP_BIN)/soapcpp2 -2jx $@ -I $(GSOAP_BASE)/import -I $(GSOAP_BASE) -I inc -d gen -f1000 -w || :
	make -j

libserver.a: $(SERVER_OBJ) $(SOAP_OBJ) | gen/onvif.h
	$(AR) rcs $@ $^

libclient.a: $(CLIENT_OBJ) $(SOAP_OBJ) | gen/onvif.h
	$(AR) rcs $@ $^

ONVIF_SRC=$(wildcard src/server*.cpp)
libonvif.a: $(ONVIF_SRC:%.cpp=%.o)
	$(AR) rcs $@ $^

# ws-discovery
libwsdd.a:
	git submodule init ws-discovery
	git submodule update ws-discovery
	make -C ws-discovery/gsoap libwsdd.a
	cp ws-discovery/gsoap/libwsdd.a .

onvif-server: src/onvif-server.o src/onvif_impl.o $(WSSE_SRC) libserver.a libonvif.a gen/soapNotificationConsumerBindingProxy.o libwsdd.a
	$(CXX) -g -o $@ $^ $(CXXFLAGS) $(LDFLAGS)


clean:
	make -C ws-discovery/gsoap clean || :
	rm -rf gen src/*.o *.a *.exe onvif-server 

install:
	mkdir -p $(DESTDIR)
	install -D -m 0755 onvif-server $(DESTDIR)
