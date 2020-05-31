# Automatic Forehead Thermometer 防疫一號改良版開源文件
[![Arduino](https://img.shields.io/badge/Arduino-v1.8.12-%2300979d)](https://www.arduino.cc/) [![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

![](https://i.imgur.com/rj6xF3J.jpg)

## 摘要
2019武漢肺炎（COVID-19)於全球擴散，疫情嚴重。臺灣靠著超前部署及科技防疫，至目前為止國內疫情仍在掌控之中。國立臺灣大學在疫情初期便採取較為嚴格的防疫措施，包含各館舍出入口門禁紀錄及額溫量測。其中額溫量測是相當費力的工作，且量測人員因近距離接觸受量測者，也可能有感染風險。防疫一號(自動額溫量測機)則可以大幅減輕量測人員的工作負擔。而此專案是基於防疫一號，加上無線傳輸功能並同時結合國立臺灣大學計算機中心的校園門禁管制系統，所完成的防疫一號進階版。功能包含讀取NFC教職員工生證件及條碼掃描身分證等，並同時量取額溫並透過WI-FI將該資料註記至資料庫中。


## 防疫一號沿革

防疫一號初代由台大電資學院 劉振森技正在2010年所研發。防疫一號是一種自動量體溫的裝置，可以在一定的距離偵測人一人體並測量體溫，不使用時可以透過簡單的計時裝置讓機器進入休眠模式以節約電力，整套系統最大的好處就是相較於熱像儀成本較低，對於學校系館，研究機構系所等而言，大部分出入口人員的出入密度相較於大型公共空間如學生餐廳、校門等並不那麼繁忙，在每個出入口都配置一台熱像儀並不符合成本效益比，而劉技正以一個簡單且便宜得多的方式達到了在疫病流行時，需要人員體溫管理的需求

之後的開發均以這個原型為基礎，在這之上增加了刷卡、QRcode、警鈴與遠端連結資料庫等功能，我們做了這些改良，嘗試著從單一台自動量體溫計單純量體溫功能，在結合門禁與資料庫系統之後，變成一個由許多體溫計組成的區域網路，達到大量人流管理與數據監控分析的目的

[台大電機系防疫一號（初代）官網說明](https://web.ee.ntu.edu.tw/news_detail.php?sn=1764)
<!-- ![](https://ee.ntu.edu.tw/upload/workbench/images/20200310_NTUEE_press_conference_109-3-19.png) -->


## 功能描述與使用流程
依照學校防疫政策，每人每日需量測額溫一次，進出各館舍時則需要刷門禁卡記錄足跡。
1. 對於今日尚未量測體溫者:
    1. 於NFC感應區刷教職員工生證件，或於條碼掃描區掃描身份證條碼。
    2. 將額頭靠近額溫槍，額溫槍透過紅外線光電開關會自動量測體溫。
    3. 體溫資料透過微控制器傳至雲端資料庫，並透過藍芽傳輸與一旁門禁管制用筆電溝通，替使用者刷卡紀錄足跡。
3. 對於今日已量測體溫者:
    1. 直接於NFC感應區刷教職員工生證件，或於條碼掃描區掃描身份證條碼。機器會透過藍芽傳輸與一旁門禁管制用筆電溝通，替使用者刷卡紀錄足跡
    
[使用情境DEMO影片](https://youtu.be/-5XMCsRwII0)

![](https://i.imgur.com/lm4z96t.jpg)

![](https://i.imgur.com/gC4Mj21.png)


## ESP32介紹(功能說明、修改分割區教學)
### PINOUT
![](https://www.botsheet.com/cht/wp-content/uploads/nodemcu-32s-eps32-pinout.jpeg?v=1587027587)

### PIN USED

| Usage | PIN | PIN | Usage |
| -------- | -------- | -------- |-------- |
| level shifter HV | 3.3 | GND | GND |
| RESET | EN      | GPIO23 | MOSI |
| RC522 RST (old) | GPIO36 | GPIO22  | LCD SCL |
|   x   | GPIO39  | GPIO1 / TX0  |    |
|   x   | GPIO34  | GPIO3 / RX0 |    |
|   x   | GPIO35  | GPIO21  |LCD SDA |
| WS2812_DATA   | GPIO32  | GND     | GND|
|  buzzer  | GPIO33  | GPIO19  |RC522 MISO |
|MOS for alarm    | GPIO25  | GPIO18  |RC522 SCK |
|MOS for alarm   | GPIO26  | GPIO5   |RC522 SDA   |
| OK | GPIO27  | GPIO17  | OK|
| OK | GPIO14  | GPIO16  |thermometer signal |
| QRCode | GPIO12  | GPIO4   |RC522 RST (new version) |
| GND| GND     | GPIO0   | OK [NO buzzer]|
| x  | GPIO13  | GPIO2   | OK|
| x  | GPIO9   | GPIO15  | OK|
| x  | GPIO10  | GPIO8   | x|
| x  | GPIO11  | GPIO7   | x|
|供電| VIN 5V | GPIO6 | x|

### 使用Arduino IED 編輯ESP32開發板程式
1. 在 偏好設定 > 額外的開發板管理員網址 輸入:
http://arduino.esp8266.com/stable/package_esp8266com_index.json

![](https://i.imgur.com/GpxSNeU.png)

2. 因完整版程式碼檔案過大，需修改ESP32磁碟分割
3. 
## 額溫槍改裝
![](https://i.imgur.com/V7QdYJW.jpg)
![](https://i.imgur.com/SV8E7vc.jpg)
使用5577額溫槍，將溫度讀值訊號線接出，在不影響額溫量測結果前提下將溫度讀出至ESP32中，並加以處理。

## 電路圖 & PCB layout & 電路說明
### PCB設計
[電路圖](https://easyeda.com/orange21201/automaticforheadthermometer)


![](https://i.imgur.com/2CDTVKk.png)

![](https://i.imgur.com/kJaku4T.png)

### 使用模組

| 名稱 | 數量 | Column 3 |
| -------- | -------- | -------- |
| ESP32 NodeMCU | 1 | Text     |
| RC522 RFID | 1 | Text     |
| 3.3V 2004 I2C LCD | 1 | Text |
| AMS1117-3.3 | 2 | Text |
| IRF540N MOSFET | 2 | Text |
| 蜂鳴器 | 1 | Text |

## Arduino Library


## 簡易版 arduino程式 與 server 說明
#### Directory
```
Basic
│
└───esp32
│    
└───server
```

#### client side(esp8266)
* library : HTTPClient.h，ArduinoJson.h
* send temperature and card uid to server
    * format `{"uid":[00,00,00,00],"temp":38.8}`
#### server side(nodejs)
* express server (port=8080)
* route 
* 1. `GET:/`
return last temperature
* 2. `POST:/upload`
upload temperature to server
## 完整版(DNS server版本) arduino程式(力維) 與 server 說明(世丞)

### Arduino 程式
* Library
    * LCD: [`LiquidCrystal_I2C`](https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library)
    * NFC: [`MFRC522`](https://github.com/miguelbalboa/rfid)
    * WS2812: [`FastLED`](https://github.com/FastLED/FastLED)
    * BLE keyboard: [`BleKeyboard`](https://github.com/T-vK/ESP32-BLE-Keyboard)

* State: `IDLE`, `WAIT_TEMP`, `SHOW_RESULT`, `NO_SHOW_RESULT`
    * `IDLE`: 待機狀態、長期閒置時向server傳送alive
    * `WAIT_TEMP`: 獲得身份認證資訊
    * `SHOW_RESULT`: 有身分認證測量、上傳資料
    * `NO_SHOW_RESULT`: 無身份認證測量、不上傳
* State transistion table

|from \ to|`IDLE`|`WAIT_TEMP`|`SHOW_RESULT`|`NO_SHOW_RESULT`|
|---|---|---|--- |---|
|`IDLE`||card/barcode detected| |temp received|
|`WAIT_TEMP`|timeout|card/barcode detected|temp received| |
|`SHOW_RESULT`|timeout|card/barcode detected||temp received|
|`NO_SHOW_RESULT`|timeout|card/barcode detected||temp received|

## 團隊成員
![](https://i.imgur.com/aOw8qNn.jpg)
#### 台大電機系 (左至右)
吳兩原、黃世丞、徐子程、施力維、陳柏志、施貽仁、劉昀昇
#### 研發場地提供
[臺大電機系 MakerSpace](https://www.facebook.com/NTUEE-Makerspace-1950706381646078)

## 特別感謝
* 臺大電資學院 張耀文院長
* 臺大電機系 吳忠幟系主任
* 臺大電機系 李宏毅教授
* 臺大計算機中心 陳啟煌工程師
* 臺大計算機中心 鄭彥宏工程師
* 臺大電機系 劉振森技正
* 旭鴻工程行 傅坤士老闆
* 臺大總務處 葛宇甯總務長
* 臺大總務處 張耀祖副理
