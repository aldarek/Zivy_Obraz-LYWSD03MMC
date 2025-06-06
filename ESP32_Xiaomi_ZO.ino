#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <WiFi.h>
#include <HTTPClient.h>
//Definice WIFI a HTTP
const char* ssid = "SSID_SITE s internetem";
const char* password = "HESLO";
const String serverUrl = "http://in.zivyobraz.eu/";  // bez otazníku na konci

//Definice hodnot posílaných na živý obraz
float lastTemp = 0;
float lastHum = 0;
float lastBat = 0;
bool dataReceived = false;



class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        

         if (advertisedDevice.getAddress().toString() == "a4:c1:38:xx:xx:xx") { //kontrola přednastavené MAC čidla
            Serial.println("✅ Nalezeno moje čidlo!");

            String serviceData = advertisedDevice.getServiceData();
            Serial.print("📦 Raw Service Data: ");
            for (char c : serviceData) Serial.printf("%02X ", (uint8_t)c);
            Serial.println();

            //vyčtení měřených hodnot z BLE service dat (formát ATC1441)


            if (serviceData.length() >= 13) {
                const uint8_t* data = (const uint8_t*)serviceData.c_str();

                float temp = ((data[6] << 8) | data[7]) / 10.0;
                uint8_t hum = data[8];
                uint16_t vbat = (data[10] << 8) | data[11];
                uint8_t batt = data[9]; // není vždy přesné
                uint8_t counter = data[12];

                //nastavení hodnot do globálních proměnných

                lastTemp = temp;
                lastHum = hum;
                lastBat = batt;
                dataReceived = true;

                Serial.printf("🌡️ Teplota: %.1f °C\n", temp);
                Serial.printf("💧 Vlhkost: %u %%\n", hum);
                Serial.printf("🔋 Napětí: %u mV\n", vbat);
                Serial.printf("🔋 Baterie (orientačně): %u %%\n", batt);
                Serial.printf("📊 Počítadlo: %u\n", counter);
            } else {
                Serial.println("❗ Service Data jsou příliš krátká.");
            }
    }
    }
};

BLEScan* pBLEScan;

void setup() {
  Serial.begin(115200);
  // Připojení k WIFI
  WiFi.begin(ssid, password);
  Serial.print("Připojování k WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi připojeno");
  //BLE Scan
    
    Serial.println("📡 Spouštím BLE skener...");

    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true); // aktivní = silnější, ale více spotřebovává
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);
    pBLEScan->start(0, nullptr, false); // 0 = skenuj stále
}

void loop() {
  if (dataReceived) {
    dataReceived = false;
  if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      // Sestavení URL pro GET požadavek
      String fullUrl = serverUrl +
                       "?import_key=VAS_IMPORT_KEY" +  // importkey z url
                       "&teplota_venku=" + String(lastTemp, 1) +
                       "&vlhkost_venku=" + String(lastHum,0) +
                       "&baterie_venku=" + String(lastBat,0);

      // Odeslání HTTP GET požadavku
      Serial.println("Odesílám na server: " + fullUrl);
      http.begin(fullUrl);
      int httpCode = http.GET();
      if (httpCode > 0) {
        Serial.println("Odpověď serveru: " + http.getString());
      } else {
        Serial.println("Chyba HTTP: " + String(httpCode));
      }
      http.end();
    }

    // Deep sleep na půl hodinu
    Serial.println("Přechod do spánku na půl hodinu...");
    esp_sleep_enable_timer_wakeup(1800000000ULL);  // 0,5 hodina
    esp_deep_sleep_start();
  }
  //čas pro dokončení BLE skenu
  delay(2000);
}
