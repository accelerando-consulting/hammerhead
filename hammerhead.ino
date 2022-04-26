#include "config.h"
#include "stacx/stacx.h"

#include "stacx/leaf_fs_preferences.h"
#include "stacx/leaf_ip_esp.h"
#include "stacx/leaf_pubsub_mqtt_esp.h"
#include "stacx/leaf_wire.h"
#include "stacx/leaf_dht12.h"
#include "stacx/leaf_shell.h"
#include "stacx/leaf_rtc.h"
//#include "stacx/leaf_sdcard.h"
#include "stacx/leaf_max44009.h"
#include "stacx/leaf_tft.h"
#include "app_hammerhead.h"

// default arrangment for an ESP8266 D1 Mini 1.4" TFT
Leaf *leaves[] = {
	new FSPreferencesLeaf("prefs"),
	new ShellLeaf("shell"),
	new IpEspLeaf("esp"),
	new PubsubEspAsyncMQTTLeaf("espmqtt"),
	new TFTLeaf("screen",1), 
	//new SDCardLeaf("sdcard"), 

	new WireBusLeaf("wire"),
	new RTCLeaf("rtc"),
	new Dht12Leaf("dht"),
	new MAX44009Leaf("lux"),
	new HammerheadAppLeaf("hammerhead", "screen,dht,lux,sdcard"),
	NULL
};

