



| Sensores | Tensão de Operação | Tensão de entrada | I operação | Pinos possíveis na ESP32             | Protocolo de Comunicação Serial |
|----------|------------|-----------|------------|--------------------------------------|---------------------------------|
| DS18B20  | 3.0 - 5.5V | 3.3V ou 5V | ~1.5mA    | GPIO4, GPIO5, GPIO15, GPIO16 ou GPIO17 | OneWire pin D04                         |
| BMP280   | 1.71 - 3.6V | 3.3V      | ~0.006mA  | GPIO21 (SDA), GPIO22 (SCL)           | I2C - pin D21                          |
| MQ-2     | 5V         | 5V        | ~150mA    | Qualquer GPIO Analógico (ex: GPIO34) | Analógico                       |
| MQ-7: Monóxido de carbono (CO)     | 3 - 5V         | 5V        | ~150mA    | Qualquer GPIO Analógico (ex: GPIO35) | Analógico                       |
| DHT-22   | 3.3 - 5V   | 3.3V ou 5V | ~1-2.5mA  | Qualquer GPIO Digital (ex: GPIO13)   | Digital pin - D13                        |
| LDR      | Variável (depende do resistor) | 3.3V ou 5V | Depende do circuito | Qualquer GPIO Analógico (ex: GPIO36) | Analógico                   |
