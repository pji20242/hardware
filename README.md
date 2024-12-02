# **Hardware**
Projeto esquemático e código fonte

## **Estrutura de mensagens**
As mensagens são enviadas no formato padrão MQTT, com o seguinte exemplo de comando:
```bash
mosquitto_pub -h IP_Broker_MQTT -p Porta_Broker_MQTT -t Tópico -d -m "UUID%1=<valor1>%2=<valor2>%3=<valor3>"
```
Exemplo prático:
```bash
mosquitto_pub -h 191.36.8.52 -p 1883 -t pji3 -d -m "rhenzo%1=2"
```


## **Simbolos Delimitadores Utilizados**
| **Simbolo** | **Uso** |
| ----------- | ------- |
| 1 - Nésimo | Código do sensores |
| % | Delimitador interno dos diferentes payload |
| = | Delimitador do Código do sensor e Informação do sensor |

## **Tabela de Sensores Utilizados** 




| Código na Comunicação | Sensores | Tensão de Operação | Tensão de entrada | I operação | Pinos Utilizados na ESP32             | Protocolo de Comunicação Serial | Parâmetro Medido |
|----------|----------|------------|-----------|------------|--------------------------------------|---------------------------------| --------------------------------|
| 1 | DS18B20  | 3.0 - 5.5V | 3.3V ou 5V | ~1.5mA    | GPIO4 | OneWire pin D04                         | Temperatura	 |
| 2 e 3 | BMP280   | 1.71 - 3.6V | 3.3V      | ~0.006mA  | GPIO21 e GPIO22 | I2C - pin D21                          | Pressão Atmosférica	 |
| 4 | MQ-2     | 5V         | 5V        | ~150mA    | GPIO5 | Analógico                       | Gás Inflamável e Fumaça	 |
| 5 | MQ-7: Monóxido de carbono (CO)     | 3 - 5V         | 5V        | ~150mA    | GPIO18 | Analógico                       | Monóxido de Carbono (CO) |
| 6 | DHT-22   | 3.3 - 5V   | 3.3V ou 5V | ~1-2.5mA  | GPIO13   | Digital pin - D13                        | Umidade |
| 7 | LDR      | Variável (depende do resistor) | 3.3V ou 5V | Depende do circuito | GPIO35 | Analógico                   | Luminosidade |
