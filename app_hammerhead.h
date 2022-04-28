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
  String rtc_date="";
  String rtc_time="";
  float temperature=0;
  float humidity=0;
  float pressure=0;
  float lux=0;
  float co2_ppm=0;
  float pm25_ugpm3=0;
  float pm10_ugpm3=0;
  TFTLeaf *screen=NULL;
  unsigned long last_log =0;
  unsigned long log_interval_sec = 60;
  

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

  void log() 
  {
    char buf[256];
    int len = snprintf(buf, sizeof(buf), "%s,%s,%.1f,%.1f,%.1f,%.1f,%1f,%.1f,%.1f",
	     rtc_date, rtc_time, temperature, humidity, pressure, lux, co2_ppm,pm25_ugpm3, pm10_ugpm3);
    LEAF_INFO("LOG %s", buf);
    buf[len++]='\n';
    buf[len]='\0';
    message("sdcard", "cmd/append/log.csv", buf);
  }
  

  
  virtual void loop(void)
  {
    AbstractAppLeaf::loop();
    unsigned long now = millis();

    if (now >= (last_log + log_interval_sec*1000)) {
      log();
      last_log = now;
    }
  }

  void draw_cmd(const char *fmt, ...) 
  {
    char cmd[128];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(cmd, sizeof(cmd), fmt, ap);
    message(screen, "cmd/draw", cmd);
  }

  void update(bool force = false) 
  {
    if (!screen) return;

    //LEAF_NOTICE("update");

    message(screen, "cmd/clear", "");
   
    int pos = rtc_time.lastIndexOf(":");
    if (pos > 0) {
      String time_str = rtc_time.substring(0, pos);
      draw_cmd("{\"font\":24,\"align\":\"center\",\"row\":8,\"column\":64,\"color\":%d, \"text\":\"%s\"}", TFT_SKYBLUE, time_str.c_str());
    }

    draw_cmd("{\"font\":8,\"align\":\"left\",\"row\": 30,\"column\": 5,\"color\":%d, \"text\":\"Temp\"}", TFT_CYAN);
    draw_cmd("{\"font\":18,\"align\":\"left\",\"row\": 38,\"column\": 5,\"color\":%d, \"text\":\"%.1fC\"}", TFT_PINK, temperature);

    draw_cmd("{\"font\":8,\"align\":\"right\",\"row\": 30,\"column\": 128,\"color\":%d, \"text\":\"Humidity\"}", TFT_CYAN);
    draw_cmd("{\"font\":18,\"align\":\"right\",\"row\": 38,\"column\": 128,\"color\":%d, \"text\":\"%.1f%%\"}",  TFT_MAGENTA, humidity);

    draw_cmd("{\"font\":8,\"align\":\"left\",\"row\": 58,\"column\": 5,\"color\":%d, \"text\":\"Lux\"}", TFT_CYAN);
    draw_cmd("{\"font\":18,\"align\":\"left\",\"row\": 66,\"column\": 5,\"color\":%d, \"text\":\"%.1f\"}", TFT_WHITE, lux);
    draw_cmd("{\"font\":8,\"align\":\"right\",\"row\": 58,\"column\": 128,\"color\":%d, \"text\":\"CO2 (ppm)\"}", TFT_CYAN);
    draw_cmd("{\"font\":18,\"align\":\"right\",\"row\": 66,\"column\": 128,\"color\":%d, \"text\":\"%.1f\"}",  TFT_GREENYELLOW, co2_ppm);

    draw_cmd("{\"font\":8,\"align\":\"left\",\"row\": 86,\"column\": 5,\"color\":%d, \"text\":\"PM2.5\"}", TFT_CYAN);
    draw_cmd("{\"font\":8,\"align\":\"center\",\"row\": 86,\"column\": 64,\"color\":%d, \"text\":\"(ug/m3)\"}", TFT_CYAN);
    draw_cmd("{\"font\":18,\"align\":\"left\",\"row\": 94,\"column\": 5,\"color\":%d, \"text\":\"%.1f\"}", TFT_YELLOW, pm25_ugpm3);
    draw_cmd("{\"font\":8,\"align\":\"right\",\"row\": 86,\"column\": 128,\"color\":%d, \"text\":\"PM10\"}", TFT_CYAN);
    draw_cmd("{\"font\":18,\"align\":\"right\",\"row\": 94,\"column\": 128,\"color\":%d,\"text\":\"%.1f\"}",  TFT_ORANGE, pm10_ugpm3);

    draw_cmd("{\"font\":10,\"align\":\"left\",\"row\": 110,\"column\": 5,\"color\":%d, \"text\":\"Hammerhead 1.0b%d\"}", TFT_GREEN, BUILD_NUMBER);
    draw_cmd("{\"font\":10,\"align\":\"right\",\"row\": 120,\"column\": 128,\"color\":%d, \"text\":\"accelerando.com.au\"}", TFT_GREEN);
  }

  virtual bool mqtt_receive(String type, String name, String topic, String payload)
  {
    LEAF_ENTER(L_DEBUG);
    bool handled = AbstractAppLeaf::mqtt_receive(type, name, topic, payload);

    LEAF_INFO("RECV %s/%s => [%s <= %s]", type.c_str(), name.c_str(), topic.c_str(), payload.c_str());

    do {
    WHEN("_ip_connect", {
	//message("pixel", "set/value", "20");
	//message("pixel", "set/hue", "4000");
      })
    WHENFROM("co2", "status/pulse", {
	int pos = payload.indexOf(",");
	if (pos < 0) {
	  LEAF_ALERT("No comma found in co2 pulse message");
	  break;
	}
	//LEAF_NOTICE("Co2 parameters pulse=[%s] interval=[%s]",
	//	    payload.substring(0,pos).c_str(),
	//	    payload.substring(pos+1));
	float pulse_ms = payload.substring(0,pos).toInt()/1000.0;
	float interval_ms = payload.substring(pos+1).toInt()/1000.0;
	LEAF_DEBUG("Co2 parameters pulse=[%.3f] interval=[%.3f]",
		    pulse_ms, interval_ms);
	co2_ppm = 2000 * (pulse_ms - 2) / (interval_ms - 4);
	LEAF_DEBUG("co2_ppm=%.1f", co2_ppm);
	//update();

      })
    WHEN("status/temperature", {
	temperature=payload.toFloat();
	update();
      })
    WHEN("status/humidity", {
	humidity=payload.toFloat();
	update();
      })
    WHEN("status/pressure", {
	pressure=payload.toFloat();
	update();
      })
    WHEN("status/lux", {
	lux=payload.toFloat();
	update();
      })
    WHEN("status/pm2d5", {
	pm25_ugpm3=payload.toFloat();
	update();
      })
    WHEN("status/pm10", {
	pm10_ugpm3=payload.toFloat();
	update();
      })
    WHEN("status/clock", {
	int pos = payload.indexOf("T");
	if (pos > 0) {
	  rtc_date = payload.substring(0,pos);
	  rtc_time = payload.substring(pos+1);
	}
	update();
      })
    else {
      LEAF_DEBUG("app did not consume type=%s name=%s topic=%s payload=%s", type.c_str(), name.c_str(), topic.c_str(), payload.c_str());
    }
    } while (0);
    
    LEAF_LEAVE;
    RETURN(handled);
  }

};

// local Variables:
// mode: C++
// c-basic-offset: 2
// End:
