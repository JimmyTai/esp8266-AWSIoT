/*
 * MQTTAWS.cpp
 *
 *  Created on: 2017年4月14日
 *      Author: GRC
 */

#include "AWSMQTT.h"

AWSMQTT::AWSMQTT(const char *endPoint, const char* deviceName, bool debugEnable, const char *certDir, const char *privateKeyDir) :
		DEBUG(debugEnable), endPoint(endPoint), certDir(certDir), privateKeyDir(privateKeyDir) {
	sprintf(mqttDeviceId, "%s-%s", deviceName, WiFi.macAddress().c_str());
	wifiClient = new WiFiClientSecure();
	client = new PubSubClient(*wifiClient);
	loadSSL();
}

AWSMQTT::~AWSMQTT() {
	delete wifiClient;
	delete client;
}

bool AWSMQTT::connect() {
	if (DEBUG)
		Serial.printf("try to connect to AWS IoT...\r\n");
	if (!WiFi.isConnected()) {
		Serial.printf("AWS IoT connecting process has been cancel: No internet connection now\r\n");
		return false;
	}
//	IPAddress remote_addr;
//	if (DEBUG)
//		Serial.printf("resolving hostname...\r\n");
//	if (!WiFi.hostByName(endPoint, remote_addr)){
//		Serial.printf("AWS IoT connecting process has been cancel: cannot resolve the hostname now\r\n");
//		return false;
//	}
	client->set_server(endPoint, 8883);
	if (DEBUG)
		Serial.printf("Attempting MQTT connection with id %s ...\r\n", mqttDeviceId);
	MQTT::Connect connectMessage(mqttDeviceId);
	connectMessage.set_keepalive(300);
	if (client->connect(connectMessage)) {
		if (DEBUG)
			Serial.printf("MQTT connected\r\n");
		return true;
	}
	if (DEBUG)
		Serial.printf("MQTT cannot connect\r\n");
	return false;
}

bool AWSMQTT::loadSSL() {
	if (!SPIFFS.begin()) {
		if (DEBUG)
			Serial.printf("failed to use file system!\r\n");
		SPIFFS.end();
		return false;
	}
	uint32_t heapUsage = ESP.getFreeHeap();
	File cert = SPIFFS.open(certDir, "r");
	if (!cert) {
		if (DEBUG)
			Serial.printf("failed to load cert file\r\n");
		SPIFFS.end();
		return false;
	}
	File privateKey = SPIFFS.open(privateKeyDir, "r");
	if (!privateKey) {
		if (DEBUG)
			Serial.printf("failed to load private key file\r\n");
		SPIFFS.end();
		return false;
	}
	if (!wifiClient->loadCertificate(cert, cert.size())) {
		if (DEBUG)
			Serial.printf("failed to set the certificate to WiFiClientSecure\r\n");
		SPIFFS.end();
		return false;
	}
	if (!wifiClient->loadPrivateKey(privateKey, privateKey.size())) {
		if (DEBUG)
			Serial.printf("failed to set the private key to WiFiClientSecure\r\n");
		SPIFFS.end();
		return false;
	}
	if (DEBUG)
		Serial.printf("load SSL success, heap usage: %d", (heapUsage - ESP.getFreeHeap()));
	return true;
}

