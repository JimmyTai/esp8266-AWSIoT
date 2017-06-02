/*
 * AWSMQTT.h
 *
 *  Created on: 2017年4月14日
 *      Author: GRC
 */

#ifndef AWSMQTT_H_
#define AWSMQTT_H_

#include "ESP8266WiFi.h"
#include "MQTT.h"
#include "PubSubClient.h"
#include "FS.h"

class AWSMQTT {
public:
	AWSMQTT(const char *endPoint, const char* deviceName, bool debugEnable = false, const char *certDir = "/cert.der", const char *privateKeyDir = "/private.der");
	~AWSMQTT();
	void setDebugOutput(bool enable) {
		this->DEBUG = enable;
	}
	PubSubClient* client;
	bool connect();

private:
	bool DEBUG;
	const char *endPoint, *certDir, *privateKeyDir;
	char mqttDeviceId[50];
	WiFiClientSecure* wifiClient;
	bool loadSSL();
};

#endif /* AWSMQTT_H_ */
