PARTITION_SCHEME=min_spiffs
#BOARD ?= espressif:esp32:ttgo-t-oi-plus
#BOARD ?= espressif:esp32:esp32c3
BOARD ?= espressif:esp32:ttgo-t7-v13-mini32

# Pins for ESP8266
# TFTPINS=-DTFT_SCLK=14 \
# 	-DTFT_MISO=12 \
# 	-DTFTF_MOSI=13 \
# 	-DTFT_CS=2 \
# 	-DTFT_DC=0 \
# 	-DTFT_RST=-1
# Pins for ESP32-C3
# TFTPINS=-DTFT_SCLK=5 \
# 	-DTFT_MISO=6 \
# 	-DTFTF_MOSI=7 \
# 	-DTFT_CS=8 \
# 	-DTFT_DC=9 \
# 	-DTFT_RST=-1

TFTPINS=-DTFT_SCLK=18 \
	-DTFT_MISO=19 \
	-DTFTF_MOSI=23 \
	-DTFT_CS=16 \
	-DTFT_DC=17 \
	-DTFT_RST=-1

TFTFONTS=-DLOAD_GLCD=1 \
	-DLOAD_FONT2=1 \
	-DLOAD_FONT4=1 \
	-DLOAD_FONT6=1 \
	-DLOAD_FONT7=1 \
	-DLOAD_FONT8=1 \
	-DLOAD_GFXFF=1 \
	-DSMOOTH_FONT=1 

CPPFLAGS=-DST7735_DRIVER=1 \
	-DTFT_WIDTH=128 \
	-DTFT_HEIGHT=160 \
	$(TFTPINS) \
	$(TFTFONTS) \
	-DSPI_FREQUENCY=27000000 \
	-DUSER_SETUP_LOADED=1

PROGRAM ?= $(shell basename $(PWD))
MAIN = $(PROGRAM).ino
SRCS = $(MAIN) \
	config.h \
	app_*.h 

# LIBS are the libraries you can install through the arduino library manager
# Format is LIBNAME[@VERSION]
LIBS =  ArduinoJson@6.14.0 \
	DHT12\ sensor\ library \
	GeekFactory\ Shell\ Library \
	Max44009 \
	NtpClientLib \
	RTClib \
	TFT_eSPI \
	Time \
	WiFiManager

# EXTRALIBS are the libraries that are not in the arduino library manager, but installed via git
# Format is LIBNAME@REPOURL
EXTRALIBS = AsyncTCP@https://github.com/me-no-dev/AsyncTCP.git \
	ESPAsyncTCP@https://github.com/me-no-dev/ESPAsyncTCP.git \
	async-mqtt-client@https://github.com/marvinroger/async-mqtt-client.git \
	SimpleMap@https://github.com/spacehuhn/SimpleMap 

#	WIFIMANAGER-ESP32@https://github.com/ozbotics/WIFIMANAGER-ESP32 

include stacx/cli.mk

