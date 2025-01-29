# **Documentação do Hardware**
Este documento detalha o hardware utilizado e fornece informações para desenvolver dispositivos próprios e que possam ser integrados 

## **Índice**

1. [Hardware Próprio](#hardware-próprio)
   - [Lista de Componentes](#lista-de-componentes)
        - [Conexões feitas com os sensores](#conexões-feitas-com-os-sensores-e-parâmetros)
   - [Esquemático Elétrico](#esquemático-elétrico)
   - [Código-Fonte](#código-fonte)
2. [Para Desenvolvedores](#para-desenvolvedores)
   - [Estrutura de Mensagens](#estrutura-de-mensagens)
   - [Protocolos Suportados](#protocolos-suportados)
   - [Sensores Compatíveis](#sensores-compatíveis)

---
## **Hardware Próprio**

### **Lista de componentes**
|**Componentes**|**Especificação**|
| ------------------- | ----------------- |
| Microcontrolador    | ESP-32            |
| Regulador de tensão | 3.3V e 5V         |
| Resistores          | 1K Ω , 470 Ω , 10K Ω    , 4.7K Ω           |
|Temperatura|DS18B20|
|Pressão Atmosférica|BMP280|
|Gás Inflamável|MQ-2|
|Fumaça|MQ-2|
|Monóxido de Carbono|MQ-7|
|Umidade|DHT-22|
|Luminosidade|LDR|

#### **Conexões feitas com os sensores e parâmetros:** 



| Código na Comunicação | Sensores | Tensão de Operação | Tensão de entrada | I operação | Pinos Utilizados na ESP32             | Protocolo de Comunicação Serial | Parâmetro Medido |
|----------|----------|------------|-----------|------------|--------------------------------------|---------------------------------| --------------------------------|
| 1 | DS18B20  | 3.0 - 5.5V | 3.3V ou 5V | ~1.5mA    | GPIO4 | OneWire pin D04                         | Temperatura	 |
| 2 | BMP280   | 1.71 - 3.6V | 3.3V      | ~0.006mA  | GPIO21 e GPIO22 | I2C - pin D21                          | Pressão Atmosférica	 |
| 3 | MQ-2     | 5V         | 5V        | ~150mA    | GPIO5 | Analógico                       | Gás Inflamável |
| 4 | MQ-2     | 5V         | 5V        | ~150mA    | GPIO5 | Analógico                       | Fumaça	 |
| 5 | MQ-7: Monóxido de carbono (CO)     | 3 - 5V         | 5V        | ~150mA    | GPIO18 | Analógico                       | Monóxido de Carbono (CO) |
| 6 | DHT-22   | 3.3 - 5V   | 3.3V ou 5V | ~1-2.5mA  | GPIO13   | Digital pin - D13                        | Umidade |
| 7 | LDR      | Variável (depende do resistor) | 3.3V ou 5V | Depende do circuito | GPIO35 | Analógico                   | Luminosidade |




### **Esquemático Elétrico**
[Esquemático Elétrico](https://raw.githubusercontent.com/pji20242/hardware/refs/heads/main/imagens/esquematico_bb.png)

### **Código-Fonte e Alterações**
O código do cliente pode ser acessado pelo repositório: [GitHub - pji20242/hardware](https://github.com/pji20242/hardware/tree/main/cliente/src)

É necessária as seguintes alterações:


## **Para Desenvolvedores**
Esta seção é destinada para aqueles desenvolvedores que desejam utilizar seus próprios dispositivos e sensores.

### **Estrutura de mensagens**
As mensagens são enviadas no formato padrão MQTT, com o seguinte exemplo de comando:
```bash
mosquitto_pub -h IP_Broker_MQTT -p Porta_Broker_MQTT -t Tópico -d -m "UUID%1=<valor1>%2=<valor2>%3=<valor3>"
```
Exemplo prático:
```bash
mosquitto_pub -h 191.36.8.52 -p 1883 -t pji3 -d -m "rhenzo%1=2"
```


### **Simbolos Delimitadores Utilizados**
| **Simbolo** | **Uso** |
| ----------- | ------- |
| 1 - Nésimo | Código do sensores |
| % | Delimitador interno dos diferentes payload |
| = | Delimitador do Código do sensor e Informação do sensor |

### **Tabela de Sensores Utilizados** 


| Código na Comunicaçao | Parâmetro Associado |
|----------|----------|
| 1 | Temperatura |
| 2 | Pressão Atmosférica |
| 3 | Gás Inflamável |
| 4 | Fumaça |
| 5 | Monóxido de Carbono (CO) |
| 6 | Umidade |
| 7 | Luminosidade |

