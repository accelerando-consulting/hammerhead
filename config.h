// Configuration defaults for Stacx
//
// Preferences can be set either by editing here, using the persistent
// storage mechanism, or via the captive-portal setup process.
//

#define BUILD_NUMBER 1

#define DEBUG_LEVEL 1
#define MAX_DEBUG_LEVEL 3

#define DEVICE_ID "hammerhead"

#ifdef ESP8266
#undef helloPin 
#else
#define helloPin 22
#endif

#define HELLO_ON 1
#define HELLO_OFF 0

#define APP_TOPIC "hammerhead"
#define DEVICE_ID_APPEND_MAC 1
#define USE_WILDCARD_TOPIC true

#define USE_TFT 1


