#include "config.h"
#include "stacx/stacx.h"

#ifdef ESP8266
#include "stacx/leaf_fs_preferences.h"
#else
#include "stacx/leaf_preferences.h"
#endif
#include "stacx/leaf_ip_esp.h"
#include "stacx/leaf_pubsub_mqtt_esp.h"
#include "stacx/leaf_wire.h"
#include "stacx/leaf_dht12.h"
#include "stacx/leaf_shell.h"
#include "stacx/leaf_rtc.h"
#include "stacx/leaf_tft.h"
#include "stacx/leaf_sdcard.h"
#include "stacx/leaf_max44009.h"
#include "stacx/leaf_sds011.h"
#include "stacx/leaf_pwmsensor.h"
#include "app_hammerhead.h"

// Pins for TTGO T-OI ESP32-C3
Leaf *leaves[] = {
#ifdef ESP8266
	new FSPreferencesLeaf("prefs"),
#else
	new PreferencesLeaf("prefs"),
#endif
	new ShellLeaf("shell"),
	new IpEspLeaf("esp","",false),
	new PubsubEspAsyncMQTTLeaf("espmqtt","",false,true,false),
	new TFTLeaf("screen",0),  // see Makefile for pin defines
	new WireBusLeaf("wire"),

	new RTCLeaf("rtc"),
	new Dht12Leaf("dht"),
	new MAX44009Leaf("lux"),

	new SDCardLeaf("sdcard"), 
	// for C3
	//new PWMSensorLeaf("co2",LEAF_PIN(2)), 
	//new SDS011SensorLeaf("dust",LEAF_PIN(4)), 
	// for ESP32
	new PWMSensorLeaf("co2",LEAF_PIN(36)), 
	new SDS011Leaf("dust",2,9600,SERIAL_8N1,26,25), 

	new HammerheadAppLeaf("hammerhead", "screen,rtc,dht,lux,sdcard,co2,dust"),
	NULL
};

