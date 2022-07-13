#define FrontLeftClockWisePin 2
#define FrontLeftPWM 3
#define FrontLeftCounterClockWisePin 4

#define FrontRightClockWisePin 5
#define FrontRightPWM 6
#define FrontRightCounterClockWisePin 7

#define RearLeftClockWisePin 8
#define RearLeftPWM 9
#define RearLeftCounterClockWisePin 10

#define RearRightClockWisePin 12
#define RearRightPWM 11
#define RearRightCounterClockWisePin 13

void pinSetup(){
    pinMode(FrontLeftClockWisePin , OUTPUT);
    pinMode(FrontLeftCounterClockWisePin , OUTPUT);

    pinMode(FrontRightClockWisePin , OUTPUT);
    pinMode(FrontRightCounterClockWisePin , OUTPUT);

    pinMode(RearLeftClockWisePin , OUTPUT);
    pinMode(RearLeftCounterClockWisePin , OUTPUT);

    pinMode(RearRightClockWisePin , OUTPUT);
    pinMode(RearRightCounterClockWisePin , OUTPUT);
}

void FLMotorController(int power){
    Serial.print("FL:");
    Serial.println(power);
    if (power > 0){
        analogWrite(FrontLeftPWM , power);
        digitalWrite(FrontLeftClockWisePin, HIGH);
        digitalWrite(FrontLeftCounterClockWisePin, LOW);
    }else if (power < 0){
        power *= -1 ;
        analogWrite(FrontLeftPWM , power);
        digitalWrite(FrontLeftClockWisePin, LOW);
        digitalWrite(FrontLeftCounterClockWisePin, HIGH);
    }else if (power == 0){
        analogWrite( FrontLeftPWM , 0);
        digitalWrite(FrontLeftClockWisePin, LOW);
        digitalWrite(FrontLeftCounterClockWisePin, LOW);
    }

}

void FRMotorController(int power){
    Serial.print("FR:");
    Serial.println(power);
    if (power > 0){
        analogWrite(FrontRightPWM , power);
        digitalWrite(FrontRightClockWisePin, HIGH);
        digitalWrite(FrontRightCounterClockWisePin, LOW);
    }else if (power < 0){
        power *= -1 ;
        analogWrite(FrontRightPWM , power);
        digitalWrite(FrontRightClockWisePin, LOW);
        digitalWrite(FrontRightCounterClockWisePin, HIGH);
    }else if (power == 0){
        analogWrite( FrontRightPWM , 0);
        digitalWrite(FrontRightClockWisePin, LOW);
        digitalWrite(FrontRightCounterClockWisePin, LOW);
    }
}

void RLMotorController(int power){
    Serial.print("RL:");
    Serial.println(power);
    if (power > 0){
        analogWrite(RearLeftPWM , power);
        digitalWrite(RearLeftClockWisePin, HIGH);
        digitalWrite(RearLeftCounterClockWisePin, LOW);
    }else if (power < 0){
        power *= -1 ;
        analogWrite(RearLeftPWM , power);
        digitalWrite(RearLeftClockWisePin, LOW);
        digitalWrite(RearLeftCounterClockWisePin, HIGH);
    }else if (power == 0){
        analogWrite( RearLeftPWM , 0);
        digitalWrite(RearLeftClockWisePin, LOW);
        digitalWrite(RearLeftCounterClockWisePin, LOW);
    }
}

void RRMotorController(int power){
    Serial.print("RR:");
    Serial.println(power);
    if (power > 0){
        analogWrite(RearRightPWM , power);
        digitalWrite(RearRightClockWisePin, HIGH);
        digitalWrite(RearRightCounterClockWisePin, LOW);
    }else if (power < 0){
        power *= -1 ;
        analogWrite(RearRightPWM , power);
        digitalWrite(RearRightClockWisePin, LOW);
        digitalWrite(RearRightCounterClockWisePin, HIGH);
    }else if (power == 0){
        analogWrite( RearRightPWM , 0);
        digitalWrite(RearRightClockWisePin, LOW);
        digitalWrite(RearRightCounterClockWisePin, LOW);
    }
}

