# Zivy_Obraz-LYWSD03MMC
Arduino kód pro ESP32, který skenuje specifické BLE MAC adresy a stáhne data z čidla LYWSD03MMC a následně odešle jako proměnné na server Živý obraz


Pro správné fungování čidla je nutné použit custom firmware : https://pvvx.github.io/ATC_MiThermometer/TelinkMiFlasher.html

Jako protokol advertising dat zvolit "ATC1441" bez ADFLAGS a Encryption


Pomocí aplikace (např. nRF Connect) lze zjistit MAC adresu čidla a zkontrolovat správný formát advertising dat.

