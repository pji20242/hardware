#ifndef  MESSAGE_H
#define MESSAGE_H

#include <string>

enum Communication_Code{
    TEMPERATURA = 1,
    ATMOSFERA = 2,
    INFLAMAVEL = 3,
    FUMACA = 4,
    MONO_CARBONO = 5,
    UMIDADE =6,
    LUMINOSIDADE = 7
};

std::string create_payload(Communication_Code code,float measure);


#endif