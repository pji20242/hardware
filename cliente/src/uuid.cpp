#include "uuid.h"

String gerarUUID() {
    uint32_t values[4];
    for (int i = 0; i < 4; ++i) {
        values[i] = esp_random();
    }

    String uuid = String(values[0], 16) + String(values[1], 16) + String(values[2], 16) + String(values[3], 16);
    return uuid;
}
