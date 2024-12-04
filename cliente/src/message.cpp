#include "message.h"

std::string create_payload(Communication_Code code, float measure){
    return "%" + std::to_string(code) + "=" + std::to_string(measure);
}