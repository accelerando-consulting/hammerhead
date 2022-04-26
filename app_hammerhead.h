//
// This class implements the display logic for an environmetal sensor
//
#pragma once

#include "stacx/abstract_app.h"

//RTC_DATA_ATTR int saved_something = 0;

class HammerheadAppLeaf : public AbstractAppLeaf
{
protected: // configuration preferences, see setup() for defaults.
  

protected: // ephemeral state
  float temperature;
  float pressure;
  TFTLeaf *screen;

public:
  HammerheadAppLeaf(String name, String target)
    : AbstractAppLeaf(name,target) {
    LEAF_ENTER(L_INFO);
    this->target=target;
    // default variables or constructor argument processing goes here
    LEAF_LEAVE;
  }

  virtual void setup(void) {
    AbstractAppLeaf::setup();
    LEAF_ENTER(L_INFO);

    screen = (TFTLeaf *)get_tap("screen");
    
    LEAF_LEAVE;
  }

  virtual void start() 
  {
    AbstractAppLeaf::start();
    
  }

  
  virtual void loop(void)
  {
    AbstractAppLeaf::loop();
  }

  void update(bool force = false) 
  {
    char cmd[128];

    LEAF_DEBUG("update");

    message(screen, "cmd/clear", "");
   
    snprintf(cmd, sizeof(cmd), "{\"font\":10,\"align\":\"center\",\"row\": 0,\"column\": 32,\"text\":\"Accelerando\",\"line\":[12,3,33,3]}");
    message(screen, "cmd/draw", cmd);

  }

  virtual bool mqtt_receive(String type, String name, String topic, String payload)
  {
    LEAF_ENTER(L_DEBUG);
    bool handled = AbstractAppLeaf::mqtt_receive(type, name, topic, payload);

    LEAF_NOTICE("RECV %s/%s => [%s <= %s]", type.c_str(), name.c_str(), topic.c_str(), payload.c_str());

    WHEN("_ip_connect", {
	//message("pixel", "set/value", "20");
	//message("pixel", "set/hue", "4000");
      })
    WHEN("status/temperature", {
	temperature=payload.toFloat();
	update();
      })
    WHEN("status/pressure", {
	pressure=payload.toFloat();
	update();
      })
    else {
      LEAF_DEBUG("app did not consume type=%s name=%s topic=%s payload=%s", type.c_str(), name.c_str(), topic.c_str(), payload.c_str());
    }
    
    LEAF_LEAVE;
    RETURN(handled);
  }

};

// local Variables:
// mode: C++
// c-basic-offset: 2
// End:
