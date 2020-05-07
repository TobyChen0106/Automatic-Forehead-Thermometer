# Automatic-Forehead-Thermometer 防疫森友會開源文件
## 摘要
## 防疫一號沿革 （暴龍）
<!-- ![](https://ee.ntu.edu.tw/upload/workbench/images/20200310_NTUEE_press_conference_109-3-19.png) -->

防疫一號初代由台大電資學院 博士劉振森 在2010年所研發，它是一種自動量體溫的裝置，可以在一定的距離偵測人一人體並測量體溫，不使用時可以透過簡單的計時裝置讓機器進入休眠模式以節約電力，整套系統最大的好處就是相較於熱像儀成本較低，對於學校系館，研究機構系所等而言，大部分出入口人員的出入密度相較於大型公共空間如學生餐廳、校門等並不那麼繁忙，在每個出入口都配置一台熱像儀並不符合成本效益比，而劉博士以一個簡單且便宜得多的方式達到了在疫病流行時，需要人員體溫管理的需求

之後的開發均以這個原型為基礎，在這之上增加了刷卡、QRcode、警鈴與遠端連結資料庫等功能，我們做了這些改良，嘗試著從單一台自動量體溫計單純量體溫功能，在結合門禁與資料庫系統之後，變成一個由許多體溫計組成的區域網路，達到大量人流管理與數據監控分析的目的

[台大電機系防疫一號（初代）官網說明](https://web.ee.ntu.edu.tw/news_detail.php?sn=1764)

## 功能描述與使用說明 (柏志)

## ESP32介紹(功能說明、修改分割區教學)(柏志)

## 電路圖 & PCB layout & 電路說明(子程)

![](https://i.imgur.com/2CDTVKk.png)
[電路圖](https://easyeda.com/orange21201/automaticforheadthermometer)


| 名稱 | 數量 | Column 3 |
| -------- | -------- | -------- |
| ESP32 NodeMCU | 1 | Text     |
| RC522 RFID | 1 | Text     |
| 3.3V 2004 I2C LCD | 1 | Text |
| AMS1117-3.3 | 2 | Text |
| IRF540N MOSFET | 2 | Text |
| 蜂鳴器 | 1 | Text |


## 簡易版 arduino程式 與 server 說明(貽仁)
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


## 其他補充說明(暴龍、兩原)