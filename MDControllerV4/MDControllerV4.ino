
#include "MotorController.hpp"
#include "PowerController.hpp"
void setup() {
  pinSetup();
  Serial.begin(115200);
  Serial.setTimeout(5);
}

//Example Command : FL128FR22RL42RR55END

void loop() {
    if(Serial.available()){
        String inputValueStr = Serial.readString();
        FLMotorController(FLPower(inputValueStr));
        FRMotorController(FRPower(inputValueStr));
        RLMotorController(RLPower(inputValueStr));
        RRMotorController(RRPower(inputValueStr));
    }
    
}
