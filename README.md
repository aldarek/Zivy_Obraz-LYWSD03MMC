# Zivy_Obraz-LYWSD03MMC 
# Arduino kód pro ESP32, který skenuje specifické BLE MAC adresy a stáhne data z čidla LYWSD03MMC a následně odešle jako proměnné na server Živý obraz 
# Potřebný HW: 
### ESP32 - dev module  
### Xiaomi LYWSD03MMC čidlo 

# Návod

Pro správné fungování čidla je nutné použit custom firmware : https://pvvx.github.io/ATC_MiThermometer/TelinkMiFlasher.html 

(pro potvrzení lze také použít https://pvvx.github.io/ATC_MiThermometer/Advertising.html ) 

Jako protokol advertising dat zvolit "ATC1441" bez ADFLAGS a Encryption


Pomocí aplikace (např. nRF Connect) lze zjistit MAC (MAC adresy čidel začínají na `A4:C1:38:XX:XX:XX`) adresu čidla a zkontrolovat správný formát advertising dat.
Service data by měla být `"Enviromental Sensing" <181A>`

Advertising data musí mít formát: `A4C1 38XX XXXX 00EA 355A 0B6E 4C`
Kde prvních 6 bajtů je MAC adresa čidla, Bajt 7 je teplota, Bajt 8 je vlhkost, Bajt 9 je baterie v procentech a Bajt 10-11 je napětí baterie v mV


Tato data se v kódu převedou a vypíšou se v seriové komunikaci.
Při úspěšném "odchycení" a "dekódování" se data pošlou na server Zivy Obraz jako proměnné (Hodnoty)


V kódu musíte upravit následující:  
```C++
const char* ssid = "SSID_SITE s internetem";  
const char* password = "HESLO";
``` 
**údaje o vaší WIFI**

```C++
if (advertisedDevice.getAddress().toString() == "a4:c1:38:xx:xx:xx") { //kontrola přednastavené MAC čidla
```
**Nastavení MAC adresy vašeho čidla**

```C++
"?import_key=VAS_IMPORT_KEY" +  // importkey z url
``` 
**Import key ze serveru živý obraz**


