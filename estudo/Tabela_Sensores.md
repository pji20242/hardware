## Tabela de Sensores para auxiliar na códificação 

| **Código** | **Sensor** | **Parâmetro Medido** | **Precisão** |
| ---------- | ---------- | -------------------- | ------------ |
| A | DS18B20 | Temperatura | 99.05% |
| B | Bmp280 | Pressão Atmosférica | $\pm$ 1 metro |
| C | MQ-2 | Gas Inflamável e Fumaça | 80% |
| D | MQ-7 | Gás Monóxido de Carbono | ?? |

## Simbolos delimitadores utilizados
| **Simbolo** | **Uso** |
| ----------- | ------- |
| A - Z | Código do sensores |
| = | Delimitador do Código e Informação do sensor |
| _ | Delimitador interno dos diferentes payload |
| @ | Delimitador final da mensagem |


Exemplos de payload
```bash
mqttClientId + "_" + "A=" + String(temperatura) + "_" + "B=" + String(umidade) + "@"
```