#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>


// for json msg parsing
StaticJsonDocument<10> doc;
DeserializationError error;

#define TIMEOUT_RX 200	 //ms
#define TIMEOUT_RFID 200 //ms

#define WIFI_NAME "<your wifi ssid>"
#define WIFI_PWD "<your wifi password>"

#define SERVER_UPLOAD_URL "http://<your server local ip>:8080/upload"


#define SS_PIN 5
#define RST_PIN 36

// #define SEN_PIN 39
// #define BTN_PIN 35 //unused

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

byte uid[4];  // RFID
char buf[13]; // thermometer rx

bool is_card_present = false;
unsigned long start_time = 0;
bool check_flag = false;

uint8_t control = 0x00;

String httpGETRequest(const char* serverName) {
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
void sendToServer(const float &temp)
{
	char str[100];
	int intArray[4];
	std::copy(uid, uid + 4, intArray);
	snprintf(str, 100, "{\"uid\":[%d,%d,%d,%d],\"temp\":%.1f}", intArray[0], intArray[1], intArray[2], intArray[3], temp);
	Serial.println(str); 
	

	if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
		
		HTTPClient http;   
		
		http.begin(SERVER_UPLOAD_URL);  //Specify destination for HTTP request
		http.addHeader("Content-Type", "application/json");
		
		int httpResponseCode = http.POST(str);
		
		if(httpResponseCode>0){
		
			String response = http.getString();                       //Get the response to the request
		
			Serial.println(httpResponseCode);   //Print return code
			Serial.println(response);           //Print request answer
		
		}else{
		
			Serial.print("Error on sending POST: ");
			Serial.println(httpResponseCode);
		
		}
		
		http.end();  //Free resources
		
	}else{
	
		Serial.println("Error in WiFi connection");   
	
	}

}

void setup()
{

	Serial.begin(115200);
	Serial2.begin(2400);
	SPI.begin();
	rfid.PCD_Init();
	for (byte i = 0; i < 6; i++)
	{
		key.keyByte[i] = 0xFF;
	}

	WiFi.begin(WIFI_NAME, WIFI_PWD);	  //連接網絡
	while (WiFi.status() != WL_CONNECTED) //等待網絡連接成功
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("WiFi connected!");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP()); //列印模塊IP

	
	Serial.println("Ok\n");
	
	delay(1000);
	
	Serial.println("ready");
}

void loop()
{
	//  get temp
	float tmp = read_tmp();

	//check card
	if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial())
	{
		start_time = millis();
		check_flag = true;
		is_card_present = true;
	}
	else
	{
		check_flag = false;
	}

	if (millis() - start_time >= TIMEOUT_RFID && check_flag == false)
	{
		is_card_present = false;
	}

	if (tmp && is_card_present)
	{
		for (byte i = 0; i < 4; i++)
		{
			uid[i] = rfid.uid.uidByte[i];
		}
		sendToServer(tmp);
	}
}

inline float raw2tmp()
{
	return (buf[6] << 8 | buf[5]) / 10.0;
}

float read_tmp()
{
	int cnt = 0;
	uint32_t last_read = millis();
	if (!Serial2.available())
		return 0;
	while (cnt < 13)
	{
		if (Serial2.available())
		{
			char c = Serial2.read();
			if (cnt == 0 && c != 0xFA)
				return 0;
			buf[cnt++] = c;
		}
		if (millis() - last_read >= TIMEOUT_RX)
			return 0;
	}
	if (cnt == 13 && buf[12] == 0xFF)
	{
		
		float tmp = raw2tmp();
		// Serial.println(tmp, 1);
		return tmp;
	}
	return 0;
}