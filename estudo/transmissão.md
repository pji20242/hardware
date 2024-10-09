# Transmissão

## Rede

| Tecnologia | Alcance | Taxa de Transmissão | Preço | Adicional |
|-|-|-|-|-|
| [Ethernet](https://infinity-cable-products.com/blogs/performance/what-is-the-cat5e-max-speed#:~:text=Cat5e%20cable%2C%20according%20to%20ANSI,10%20meters%20for%20patch%20cables) | ~100 metros | 10 Mbps - 1 Gbps | | | 
| [Wi-fi](https://en.wikipedia.org/wiki/IEEE_802.11) | ~50 metros (2,4 GHz) | 11 Mbps - 1,3 Gbps (Wi-Fi 5) | Módulos Wi-Fi para sistemas embarcados, como [ESP8266](https://lista.mercadolivre.com.br/esp8266), [ESP32](https://lista.mercadolivre.com.br/esp32): R$ 15,00 - R$ 100,00. | Flexível para IoT. Mais interferências, consumo energético médio. |
| [Bluetooth](https://www.bluetooth.com/learn-about-bluetooth/key-attributes/range/) | [10-100 metros](https://www.bluetooth.com/learn-about-bluetooth/tech-overview/) | 1-3 Mbps (Bluetooth 4.0) | Módulos Bluetooth, como [HC-05](https://www.eletrogate.com/modulo-bluetooth-rs232-hc-05), [HC-06](https://www.eletrogate.com/modulo-bluetooth-rs232-hc-06): R$ 20,00 - R$ 50,00. | Consumo energético baixo, usado em dispositivos pessoais. |Alcance depende do perfil de potência.
| [Zigbee](https://www.digi.com/products/embedded-systems/digi-xbee/rf-modules/2-4-ghz-rf-modules/xbee-zigbee#specifications) | 10-100 metros | 250 Kbps | [Módulos Zigbee como XBee](https://www.mercadolivre.com.br/modulo-xbee-s2-s2c-zigbee-antena-wire-digi-xbee/p/MLB36590897#polycard_client=search-nordic&wid=MLB3680705561&sid=search&searchVariation=MLB36590897&position=3&search_layout=grid&type=product&tracking_id=fb97a5b0-a53b-4830-9123-0a9ec09e166e): R$ 150,00 - R$ 300,00. | Ideal para IoT e automação residencial, baixo consumo de energia, usado em redes de sensores. | 
| [LoRa](https://en.wikipedia.org/wiki/LoRa) | 5-15 km em áreas abertas | 0,3 - 27 Kbps | Módulos LoRa, como [RFM95](https://produto.mercadolivre.com.br/MLB-1737741200-modulo-lora-915-mhz-rfm95-_JM), [SX1278](https://produto.mercadolivre.com.br/MLB-3703680719-modulo-lora-sx1278-433mhz-ra-02-longa-distncia-iot-_JM#is_advertising=true&position=2&search_layout=grid&type=pad&tracking_id=0811d767-c06f-4764-9a46-8bf0257f185a&is_advertising=true&ad_domain=VQCATCORE_LST&ad_position=2&ad_click_id=ZjNlNGQ2ZWYtMjZkNS00ZGViLWEwMDktYTcyYzY4MGRhMWZm): R$ 50,00 - R$ 200,00. | Longo alcance e baixa taxa de transmissão, usado em comunicações M2M e IoT. | 
GSM, LTE, 5G | Área de cobertura por operadora | Depende da tecnologia e plano assinado | módulos GSM, LTE, como [SIM800L](https://lista.mercadolivre.com.br/modulo-sim800l), SIM900: R$ 50,00 - R$ 150,00. |
 
## Protocolos

| Protocolo | Consumo de Energia | Escalabilidade | Confiabilidade | Tamanho do Pacote |
|-|-|-|-|-
| [MQTT](https://www.hivemq.com/blog/mqtt-essentials-part-1-introducing-mqtt/) | Baixo | Alta | Média | 256 bytes (LoRa) / 64 KB (IEEE 802) |
| [CoAp](https://datatracker.ietf.org/doc/html/rfc7252) | Muito baixo | Média | Média | 4 bytes até 1 KB |
| [HTTP](https://datatracker.ietf.org/doc/html/rfc2616) | Médio | Baixa | Alta | |
| [WebSocket](https://datatracker.ietf.org/doc/html/rfc6455) | Médio | Média | Alta | | 
| AMQP | Médio | Alta | Alta | | 

## Placa Microcontroladora

| Microcontroladora | Consumo de Energia | Conectividade | Compatibilidade de Sensores | Facilidade de Uso | Recursos Extras | Preço (BRL) |
|-|-|-|-|-|-|-|
| [ESP32](https://www.espressif.com/en/products/socs/esp32/[ESP8266](https://www.espressif.com/en/products/socs/esp8266) | Moderado, com bons modos de economia de energia | Wi-Fi, Bluetooth | I2C, SPI, UART ([ESP32](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/), [ESP8266](https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/)) | Fácil (Arduino IDE, ESP-IDF) | Processamento dual-core | R$ 35 - R$ 70 ([ESP32](https://lista.mercadolivre.com.br/esp32), [ESP8266](https://lista.mercadolivre.com.br/esp8266#D[A:ESP8266)) |
| [STM32](https://www.st.com/en/microcontrollers-microprocessors/stm32-ultra-low-power-mcus.html) (Série de Baixo Consumo) | Muito baixo, com modos de ultrabaixo consumo de energia | Várias opções (LoRa com módulo adicional) | I2C, SPI, UART | Média (STM32CubeMX) | Periféricos avançados | [R$ 50 - R$ 150](https://lista.mercadolivre.com.br/stm32-l4) |
| [Arduino MKR WAN 1310](https://docs.arduino.cc/hardware/mkr-wan-1310) | Baixo, eficiente em energia | LoRa (integrado) | I2C, SPI, UART | Muito fácil (Arduino IDE) | Carregamento de bateria integrado | [R$ 250 - R$ 400](https://store.arduino.cc/products/arduino-mkr-wan-1310) | 
| [Adafruit Feather M0 LoRa](https://learn.adafruit.com/adafruit-feather-m0-radio-with-lora-radio-module/overview) | Baixo, adequado para alimentação por bateria | LoRa (integrado) ([Ref.](https://learn.adafruit.com/adafruit-feather-m0-radio-with-lora-radio-module/pinouts)) | I2C, SPI, UART | Fácil (Arduino IDE) | Leve e compacto | [R$ 160 - R$ 300](https://www.adafruit.com/product/3178) |


### Referências 
- [Embedded - Protocols | ArunEworld](https://aruneworld.com/embedded/embedded-protocol/)
- [LTE-M for IoT and M2M: Everything You Need to Know | Eseye](https://www.eseye.com/resources/iot-explained/lte-m-for-iot-m2m/)
- [Internet of Things Networks: standards for short and long range](https://www.cs.unibo.it/projects/iot/IoT_standards2.pdf)
- [O Incrível STM32 L4! - Fernando K Tecnologia](https://www.fernandok.com/2018/06/o-incrivel-stm32-l4.html)
- [An Embedded Application System for Data Collection of Atmospheric Pollutants with a Classification Approach](https://sci-hub.se/10.1007/978-3-319-27060-9_46)
- [An Embedded System Application to Monitoring Micro-climates Oriented to Smart Cities](https://sci-hub.se/10.1109/ISC2.2015.7366200)
- [Global Surface Albedo Product Validation Best Practices Protocol](https://lpvs.gsfc.nasa.gov/PDF/CEOS_ALBEDO_Protocol_20190307_v1.pdf)
- [Waspmote - Libelium](https://www.libelium.com/iot-products/waspmote/)
