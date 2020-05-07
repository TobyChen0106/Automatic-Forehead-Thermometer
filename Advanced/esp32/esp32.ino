/********************************
* CHANGE THIS FOR EVERY STATION *
*********************************/
#define STATION_ID 1000
#define STATION_NAME "EP2-000"
/********************************
* CHANGE THIS FOR EVERY STATION *
*********************************/
#define BLE_KEYBOARD

#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <FastLED.h>
#include "lcd_display.h"

//QRCode
#include <SoftwareSerial.h>
SoftwareSerial qrcodeSerial(12, 14); //rx, tx
#ifdef BLE_KEYBOARD
#include <BleKeyboard.h>
BleKeyboard bleKeyboard(STATION_NAME, "NTUEE - EP Crossing Team");
#endif

// timeouts
#define TIMEOUT_RX 200	 //ms
#define TIMEOUT_RFID 200 //ms
// server
#define PERIOD_ALIVE_REPORT 3 * 60 * 1000 // minutes to ms

#define WIFI_NAME "YOUR_WIFI_NAME"
#define WIFI_PWD "YOUR_WIFI_PWD"
#define WIFI_TimeOut 10000

#define DOMAIN_POSTFIX ".YOUR.DOMAIN.NAME.POSTFIX"

// wait
#define WAIT_TMP 8000
#define WAIT_ERROR 5000
#define WAIT_SHOW 5000
// pins
#define PIN_GREEN 25
#define PIN_RED 26
#define PIN_BUZZER 33
#define PIN_SS 5
#define PIN_RST 36
#define PIN_WS 32
#define PIN_WIFI_MODE 15
//
#define DELAY_LOW_TMP 25
#define DELAY_HIGH_TMP 4000
#define LOW_TMP 32
#define HIGH_TMP 37.5
// state
#define STATE_IDLE 0
#define STATE_WAIT_TMP 1
#define STATE_SHOW_RESULT 2
#define STATE_NO_CARD_RESULT 3
#define STATE_ERROR 4
// ws2812
#define NUM_LEDS 7
#define WS_COLOR  \
	{             \
		0, 255, 0 \
	}
// sendToServer status code
#define INIT_ERR 0
#define INIT_FAIL 1
#define INIT_OK 2
#define TRANS_QUARA 3
#define TRANS_ERR 4
#define TRANS_OK 5

#define QRCODE_LENGTH 100
unsigned long last_alive_report = 0;

// set the LCD number of columns and rows
LiquidCrystal_I2C lcd(0x3f, 20, 4);

// rfid
MFRC522 rfid(PIN_SS, PIN_RST);
MFRC522::MIFARE_Key key;
// ws2812
CRGB leds[NUM_LEDS];

byte uid[4];   // RFID
char buf[13];  // thermometer rx
float tmp = 1; // thermometer temperature

int show_state = 0;
unsigned long state_timer;
char QRCode[QRCODE_LENGTH] = "";
int input_flag = 0; // 0 for no input, 1 for NFC, 2 for QRCode
int sendToServer()
{
	char str[200] = "";
	char query_buffer[250] = "";
	// String _qrcode;
	if (input_flag == 2)
	{
		sprintf(str, "%s,%d,%d", QRCode, (int)(tmp * 10), STATION_ID);
	}
	else if (input_flag == 1)
	{
		uint32_t intArray[4];
		std::copy(uid, uid + 4, intArray);
		uint32_t decID = (intArray[3] << 24) + (intArray[2] << 16) + (intArray[1] << 8) + (intArray[0]);
		sprintf(str, "%010lu,%d,%d", decID, (int)(tmp * 10), STATION_ID);
	}
	else
	{
		return -1;
	}
	Serial.println(str);

	IPAddress foo;
	unsigned long current_time = millis();
	sprintf(query_buffer, "c,%s,%d%s", str, current_time, DOMAIN_POSTFIX);
	if (WiFi.hostByName(query_buffer, foo))
	{
		Serial.print(F("[i] Server response: "));
		foo.printTo(Serial);
		Serial.println();
		last_alive_report = millis();
		if (foo[0] == 1 && foo[1] == 1 && foo[2] == 1 && foo[3] == 1)
		{
			return TRANS_OK;
		}
		else if (foo[0] == 2 && foo[1] == 2 && foo[2] == 2 && foo[3] == 2)
		{
			return TRANS_ERR;
		}
		else if (foo[0] == 3 && foo[1] == 3 && foo[2] == 3 && foo[3] == 3)
		{
			return TRANS_QUARA;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

void setup_LCD()
{
	lcd.init();
	lcd.backlight();
	// assignes each segment a write number
	lcd.createChar(8, LT);
	lcd.createChar(1, UB);
	lcd.createChar(2, RT);
	lcd.createChar(3, LL);
	lcd.createChar(4, LB);
	lcd.createChar(5, LR);
	lcd.createChar(6, UMB);
	lcd.createChar(7, LMB);
}

void setup()
{
	// BLE KEYBOARD
#ifdef BLE_KEYBOARD
	bleKeyboard.begin();
#endif

	// Buzzer
	ledcSetup(0, 1E5, 12);
	ledcAttachPin(PIN_BUZZER, 0);

	//LCD
	setup_LCD();
	lcd_display("Initializing...", 'm', lcd, 1, 0);

	pinMode(PIN_GREEN, OUTPUT);
	pinMode(PIN_RED, OUTPUT);
	digitalWrite(PIN_GREEN, 0);
	digitalWrite(PIN_RED, 0);
	// ws2812 init
	FastLED.addLeds<NEOPIXEL, PIN_WS>(leds, NUM_LEDS);
	FastLED.setBrightness(128);
	// serial init
	Serial.begin(115200);
	qrcodeSerial.begin(9600);
	Serial2.begin(2400);
	// rfid init
	SPI.begin();
	rfid.PCD_Init();
	for (byte i = 0; i < 6; i++)
	{
		key.keyByte[i] = 0xFF;
	}
	
	// wifi init
	pinMode(PIN_WIFI_MODE, INPUT_PULLUP);
	bool wifi_mode = digitalRead(PIN_WIFI_MODE);
	if (wifi_mode)
	{
		lcd_display("WIFI Connecting...", 'm', lcd, 2, 0);
		Serial.println("WIFI Connecting...");
		WiFi.begin(WIFI_NAME, WIFI_PWD); //連接網絡
		lcd.setCursor(0, 3);
		while (WiFi.status() != WL_CONNECTED) //等待網絡連接成功
		{
			delay(500);
			Serial.print(".");
			lcd.print(".");
		}
		Serial.println("WiFi connected!");
		Serial.print("IP address: ");
		Serial.println(WiFi.localIP()); //列印模塊IP
		lcd.clear();
		lcd_display("WIFI Connected!!", 'm', lcd, 2, 0);
	}
	else
	{
		lcd_display("Using HotSpot WIFI..", 'm', lcd, 2, 0);
		Serial.println("Using HotSpot WIFI..");

		// BACKUP_WIFI
		char BACKUP_WIFI_NAME[5];
		char BACKUP_WIFI_PWD[8];
		// Fill in your backup wifi name and password
		sprintf(BACKUP_WIFI_NAME, "%s", "BACKUP_WIFI_NAME");
		sprintf(BACKUP_WIFI_PWD, "%08d", "BACKUP_WIFI_PWD");
		Serial.print("BACKUP_WIFI_NAME: ");
		Serial.println(BACKUP_WIFI_NAME);
		Serial.print("BACKUP_WIFI_NAME: ");
		Serial.println(BACKUP_WIFI_PWD);
		WiFi.begin(BACKUP_WIFI_NAME, BACKUP_WIFI_PWD); //連接網絡

		lcd.setCursor(0, 3);
		while (WiFi.status() != WL_CONNECTED) //等待網絡連接成功
		{
			delay(500);
			Serial.print(".");
			lcd.print(".");
		}
		Serial.println("WiFi connected!");
		Serial.print("IP address: ");
		Serial.println(WiFi.localIP()); //列印模塊IP
		lcd.clear();
		lcd_display("WIFI Reconnected!!", 'm', lcd, 2, 0);
	}

	Serial.println("Ok\n");
	Serial.println("Starting client\n");
	delay(200);
	lcd_display("Reporting...", 'm', lcd, 3, 0);
	reportAlive();
	lcd.clear();
	Serial.println("ready");
	display_ready(lcd);

	delay(800);
	gotoState(STATE_IDLE);
}

void loop()
{

	tmp = read_tmp();
	int card = detectCard();
	int code = detectQRCode();

	if (tmp)
		Serial.println(tmp, 1);
	if (card)
		printUID();
	else if (code)
		Serial.println(QRCode);

	switch (show_state)
	{
	case STATE_IDLE:
		if ((millis() - last_alive_report) > PERIOD_ALIVE_REPORT)
		{
			reportAlive();
			gotoState(STATE_IDLE);
		}
		if (card)
		{
#ifdef BLE_KEYBOARD
			BLE_SendUID();
#endif
			input_flag = 1;
			gotoState(STATE_WAIT_TMP);
		}
		else if (code)
		{
#ifdef BLE_KEYBOARD
			BLE_SendQRCode();
#endif
			input_flag = 2;
			gotoState(STATE_WAIT_TMP);
		}
		else if (tmp)
		{
			input_flag = 0;
			gotoState(STATE_NO_CARD_RESULT);
		}
		break;

	case STATE_WAIT_TMP:
		showWS2812((millis() - state_timer));
		showBuzzer((millis() - state_timer));
		if (tmp)
		{
			ledcWrite(0, 0);
			gotoState(STATE_SHOW_RESULT);
		}
		else if (card)
		{
#ifdef BLE_KEYBOARD
			BLE_SendUID();
#endif
			ledcWrite(0, 0);
			input_flag = 1;
			gotoState(STATE_WAIT_TMP);
		}
		else if (code)
		{
#ifdef BLE_KEYBOARD
			BLE_SendQRCode();
#endif
			input_flag = 2;
			gotoState(STATE_WAIT_TMP);
		}
		else if ((millis() - state_timer) > WAIT_TMP)
		{
			ledcWrite(0, 0);
			input_flag = 0;
			gotoState(STATE_IDLE);
		}
		break;

	case STATE_SHOW_RESULT:
		if (card)
		{
#ifdef BLE_KEYBOARD
			BLE_SendUID();
#endif
			input_flag = 1;
			gotoState(STATE_WAIT_TMP);
		}
		else if (code)
		{
#ifdef BLE_KEYBOARD
			BLE_SendQRCode();
#endif
			input_flag = 2;
			gotoState(STATE_WAIT_TMP);
		}
		else if (tmp)
		{
			input_flag = 0;
			gotoState(STATE_NO_CARD_RESULT);
		}
		else if ((millis() - state_timer) > WAIT_SHOW)
		{
			input_flag = 0;
			gotoState(STATE_IDLE);
		}
		break;

	case STATE_NO_CARD_RESULT:
		if (card)
		{
#ifdef BLE_KEYBOARD
			BLE_SendUID();
#endif
			input_flag = 1;
			gotoState(STATE_WAIT_TMP);
		}
		else if (code)
		{
#ifdef BLE_KEYBOARD
			BLE_SendQRCode();
#endif
			input_flag = 2;
			gotoState(STATE_WAIT_TMP);
		}
		else if (tmp)
		{
			input_flag = 0;
			gotoState(STATE_NO_CARD_RESULT);
		}
		else if ((millis() - state_timer) > WAIT_SHOW)
		{
			input_flag = 0;
			gotoState(STATE_IDLE);
		}
		break;

	case STATE_ERROR:
		if ((millis() - state_timer) > WAIT_ERROR)
		{
			input_flag = 0;
			gotoState(STATE_IDLE);
		}
		break;
	default:
		input_flag = 0;
		gotoState(STATE_IDLE);
		break;
	}
}

inline float raw2tmp()
{
	return (buf[6] << 8 | buf[5]) / 10.0;
}

float read_tmp()
{
	// tmp = 0;
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
		return raw2tmp();
	}
	return 0;
}

bool reportAlive()
{
	char query_buffer[128] = "";
	Serial.print(F("[i] Reporting alive.\n"));
	last_alive_report = millis();
	IPAddress foo;
	sprintf(query_buffer, "a,%d,%lu%s", STATION_ID, millis(), DOMAIN_POSTFIX);
	if (WiFi.hostByName(query_buffer, foo))
	{
		lcd_display("Connected to Server", 'm', lcd, 3, 0);
		Serial.print(F("Connected to Server.\n"));
		return 1;
	}
	else
	{
		// gotoState(STATE_ERROR);
		lcd_display("Not connected to Server", 'm', lcd, 3, 0);
		Serial.print(F("Not connected to Server.\n"));
		return 0;
	}
}

void gotoState(int state)
{
	char tmp_str[5];
	int serverReturn = -1;
	switch (state)
	{
	case STATE_IDLE:
		Serial.println("State: idle");

		digitalWrite(PIN_GREEN, 0);
		digitalWrite(PIN_RED, 0);
		offWS2812();

		lcd.clear();
		display_card(lcd);
		state_timer = millis();
		break;

	case STATE_WAIT_TMP:
		Serial.println("State: wait_tmp");
		lcd.clear();
		digitalWrite(PIN_GREEN, 0);
		digitalWrite(PIN_RED, 0);
		display_temp(lcd);
		state_timer = millis();
		break;

	case STATE_SHOW_RESULT:
		Serial.println("State: show_result");
		offWS2812();
		lcd.clear();
		lcd_display_tmp(tmp, lcd);
		turnLED(tmp);

		state_timer = millis();

		if (tmp < LOW_TMP)
		{
			lcd_display("Low Body Temperature", 'm', lcd, 0, 0);
			lcd_display("Please Try Again....", 'm', lcd, 3, 0);
			break;
		}

		serverReturn = sendToServer();
		if (serverReturn == 5)
		{
			Serial.println("Data Saved to Server!");
			lcd_display("Data Saved to Server", 'm', lcd, 3, 0);
		}
		else if (serverReturn == 4)
		{
			Serial.println("Verification Error!");
			lcd_display("Verification Error!", 'm', lcd, 3, 0);
		}
		else if (serverReturn == 3)
		{
			Serial.println("User On Quarantine!");
			lcd_display("In Quarantine!", 'm', lcd, 3, 0);
			digitalWrite(PIN_RED, 1);
			delay(DELAY_LOW_TMP);
			digitalWrite(PIN_RED, 0);
		}
		else // ERROR
		{
			// gotoState(STATE_ERROR);
			Serial.println("send DNS error");
			lcd_display("Data Saved to Server", 'm', lcd, 3, 0);
			// lcd_display("Send DNS Error!", 'm', lcd, 3, 0);
		}
#ifdef BLE_KEYBOARD
		delay(550);
		if (input_flag == 2)
			BLE_SendQRCode();
		else if (input_flag == 1)
			BLE_SendUID();
#endif
		break;

	case STATE_NO_CARD_RESULT:
		Serial.println("State: no_card_result");

		lcd.clear();
		lcd_display_tmp(tmp, lcd);
		lcd_display("no card", 'm', lcd, 3, 0);

		turnLED(tmp);

		state_timer = millis();
		break;

	case STATE_ERROR:
		Serial.println("State: error");

		lcd.clear();
		display_err(lcd);

		state_timer = millis();
		break;

	default:
		lcd.clear();
		lcd_display("ERROR", 'm', lcd, 0, 0);
		lcd_display("State number error", 'm', lcd, 1, 0);
		break;
	}
	show_state = state;
}
int detectQRCode()
{
	int index = 0;
	if (qrcodeSerial.available())
	{
		unsigned long qrStart = millis();
		while (millis() - qrStart < 100 && index < QRCODE_LENGTH - 1)
		{
			if (qrcodeSerial.available())
			{
				qrStart = millis();
				QRCode[index] = (char)(qrcodeSerial.read());
				index++;
			}
		}
		QRCode[index] = '\0';
		return 1;
	}
	return 0;
}
int detectCard()
{
	int detect_type = 0;

	if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
		return 0;

	// Check is the PICC of Classic MIFARE type
	MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
	if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
		piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
		piccType != MFRC522::PICC_TYPE_MIFARE_4K)
	{
		// Serial.println(F("Your tag is not of type MIFARE Classic."));
		return 0;
	}

	if (rfid.uid.uidByte[0] != uid[0] ||
		rfid.uid.uidByte[1] != uid[1] ||
		rfid.uid.uidByte[2] != uid[2] ||
		rfid.uid.uidByte[3] != uid[3])
	{
		// Serial.println(F("A new card has been detected."));
		detect_type = 1;

		// Store NUID into uid array
		for (byte i = 0; i < 4; i++)
		{
			uid[i] = rfid.uid.uidByte[i];
		}
	}
	else
	{
		// Serial.println(F("Card read previously."));
		detect_type = 2;
	}
	// Halt PICC
	rfid.PICC_HaltA();
	// Stop encryption on PCD
	rfid.PCD_StopCrypto1();

	return detect_type;
}

void turnLED(float tmp)
{
	if (tmp < LOW_TMP)
	{
		digitalWrite(PIN_GREEN, 0);
		digitalWrite(PIN_RED, 1);
		delay(DELAY_LOW_TMP);
		digitalWrite(PIN_RED, 0);
	}
	else if (tmp >= HIGH_TMP)
	{
		digitalWrite(PIN_GREEN, 0);
		digitalWrite(PIN_RED, 1);
		delay(DELAY_HIGH_TMP);
		digitalWrite(PIN_RED, 0);
	}
	else
	{
		digitalWrite(PIN_RED, 0);
		digitalWrite(PIN_GREEN, 1);
	}
}

void showWS2812(long dt)
{
	int c[] = WS_COLOR;
	int time_per_led = WAIT_TMP / NUM_LEDS;
	int nled = (WAIT_TMP - dt) / time_per_led + 1;
	if (dt > WAIT_TMP)
		nled = 0;
	for (int i = 0; i < nled - 1; ++i)
		leds[i] = CRGB(c[0], c[1], c[2]);
	for (int i = 0; i < 3; ++i)
		c[i] = c[i] * ((WAIT_TMP - dt) % (time_per_led)) / time_per_led;
	leds[nled - 1] = CRGB(c[0], c[1], c[2]);
	for (int i = nled; i < NUM_LEDS; ++i)
		leds[i] = CRGB::Black;
	FastLED.show();
}
void showBuzzer(long dt)
{
	if (dt < 200)
	{
		ledcWriteTone(0, 1200);
	}
	else
	{
		ledcWrite(0, 0);
	}
}
void offWS2812()
{
	for (int i = 0; i < NUM_LEDS; ++i)
		leds[i] = CRGB::Black;
	FastLED.show();
}

void printUID()
{
	Serial.print(F("UID:"));
	for (byte i = 0; i < rfid.uid.size; i++)
	{
		String prefix = rfid.uid.uidByte[i] < 0x10 ? " 0" : " ";
		String hex = String(rfid.uid.uidByte[i], HEX);
		hex.toUpperCase();
		Serial.print(prefix);
		Serial.print(hex);
	}
	Serial.println();
}

void BLE_SendUID()
{
#ifdef BLE_KEYBOARD
	if (bleKeyboard.isConnected())
	{
		char str[11];
		uint32_t intArray[4];
		std::copy(uid, uid + 4, intArray);
		uint32_t decID = (intArray[3] << 24) + (intArray[2] << 16) + (intArray[1] << 8) + (intArray[0]);

		sprintf(str, "%010lu", decID);
		bleKeyboard.print(str);
		bleKeyboard.write(KEY_RETURN);
		delay(30);
	}
#endif
}

void BLE_SendQRCode()
{
#ifdef BLE_KEYBOARD
	Serial.print(QRCode);

	int _count;
	for (_count = 0; QRCode[_count] != '\0'; ++_count)
	{
	};

	if (bleKeyboard.isConnected())
	{
		bleKeyboard.print(QRCode);
		delay(_count * 20);
		bleKeyboard.write(KEY_RETURN);
		delay(20);
	}
#endif
}
