
---

## **Estrutura de Mensagens**

| **Protocolo** | **Formato de Mensagem** | **Aplicação** | **Tamanho de Pacote** | **Referência** |
|---------------|-------------------------|---------------|-----------------------|----------------|
| MQTT          | JSON ou binário         | IoT, baixa largura de banda | 1-256 bytes | [MQTT.org](https://mqtt.org/mqtt-specification/),  [RFC9431](https://datatracker.ietf.org/doc/rfc9431/) |
| LoRaWAN       | Payload Compactado      | Comunicação de longa distância | 11-242 bytes | [LoRa Alliance](https://resources.lora-alliance.org/technical-specifications), [Datatracker](https://datatracker.ietf.org/doc/draft-farrell-lpwan-lora-overview/)|
| CoAP          | RESTful, UDP            | Sensores remotos, IoT | 1-64 bytes | [CoAP Protocol](https://datatracker.ietf.org/doc/html/rfc7252), [RFC7252](https://datatracker.ietf.org/doc/html/rfc7252) |
| AMQP          | Binário estruturado     | Mensagens de alta confiabilidade | 64-512 bytes | [AMQP](https://www.amqp.org), [RabbitMQ](https://www.rabbitmq.com/) |

---

