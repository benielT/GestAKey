/*********************************************************
This is the arduino code for GestKey to getting the touching data

Written by Yilei
**********************************************************/

#include <Wire.h>
#include "Adafruit_MPR121.h"

#define KEY_A 0
#define KEY_B 11
#define KEY_C 8
#define KEY_D 2
#define KEY_E 14
#define KEY_F 3
#define KEY_G 5
#define KEY_H 4
#define KEY_I 34
#define KEY_J 38
#define KEY_K 40
#define KEY_L 37
#define KEY_M 46
#define KEY_N 45
#define KEY_O 31
#define KEY_P 35
#define KEY_Q 12
#define KEY_R 15
#define KEY_S 1
#define KEY_T 17
#define KEY_U 32
#define KEY_V 9
#define KEY_W 13
#define KEY_X 7
#define KEY_Y 16
#define KEY_Z 6
#define KEY_DELETE 51
#define KEY_FN 59

#define MULTIPLEXER_Q 0x70
#define MULTIPLEXER_A 0x70
#define MULTIPLEXER_Z 0x70
#define MULTIPLEXER_W 0x70
#define MULTIPLEXER_S 0x70
#define MULTIPLEXER_X 0x70
#define MULTIPLEXER_E 0x71
#define MULTIPLEXER_D 0x71
#define MULTIPLEXER_C 0x71
#define MULTIPLEXER_R 0x71
#define MULTIPLEXER_F 0x71
#define MULTIPLEXER_V 0x71
#define MULTIPLEXER_T 0x72
#define MULTIPLEXER_G 0x72
#define MULTIPLEXER_B 0x72
#define MULTIPLEXER_Y 0x72
#define MULTIPLEXER_H 0x72
#define MULTIPLEXER_N 0x72
#define MULTIPLEXER_U 0x73
#define MULTIPLEXER_J 0x73
#define MULTIPLEXER_M 0x73
#define MULTIPLEXER_I 0x73
#define MULTIPLEXER_K 0x73
#define MULTIPLEXER_O 0x74
#define MULTIPLEXER_L 0x74
#define MULTIPLEXER_P 0x74
#define MULTIPLEXER_DELETE 0x74
#define MULTIPLEXER_FN 0x74

#define I2C_PIN_Q 2
#define I2C_PIN_A 3
#define I2C_PIN_Z 4
#define I2C_PIN_W 5
#define I2C_PIN_S 6
#define I2C_PIN_X 7
#define I2C_PIN_E 2
#define I2C_PIN_D 3
#define I2C_PIN_C 4
#define I2C_PIN_R 5
#define I2C_PIN_F 6
#define I2C_PIN_V 7
#define I2C_PIN_T 2
#define I2C_PIN_G 3
#define I2C_PIN_B 4
#define I2C_PIN_Y 5
#define I2C_PIN_H 6
#define I2C_PIN_N 7
#define I2C_PIN_U 2
#define I2C_PIN_J 3
#define I2C_PIN_M 4
#define I2C_PIN_I 5
#define I2C_PIN_K 6
#define I2C_PIN_O 2
#define I2C_PIN_L 3
#define I2C_PIN_P 5
#define I2C_PIN_DELETE 7
#define I2C_PIN_FN 6


int TCA_ADD = 0x70;
int I2C_NUM = 0;
int key;
int light = 1;

// You can have up to 4 on one i2c bus but one is enough for testing!
//GND - 0x5A
//VIN - 0x5B
//SDA - 0x5C
//SCL - 0x5D
Adafruit_MPR121 cap = Adafruit_MPR121();

void tcaselect(uint8_t i){
  //if(i > 7) return;
  Wire.beginTransmission(TCA_ADD);
  Wire.write(1 << i);
  Wire.endTransmission(); 
}

void setup() {

  pinMode(A0, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  
  while (!Serial);
  Wire.begin();
  Serial.begin(115200);
  Serial.setTimeout(10);
  tcaselect(2);
  while (!cap.begin(0x5A)) {
    Serial.println("A not found, check wiring?");
  }
}

void loop() {
  
  if(Serial.available()>0){
    String key = Serial.readString();
    
    light = -light;
    if(light == 1){
      digitalWrite(LED_BUILTIN, HIGH);
    }else{
      digitalWrite(LED_BUILTIN, LOW);
    }
    
    switch (key.toInt()) {   
    
    case KEY_A:
      TCA_ADD = MULTIPLEXER_A;
      I2C_NUM = I2C_PIN_A;
      break;
    case KEY_B:
      TCA_ADD = MULTIPLEXER_B;
      I2C_NUM = I2C_PIN_B;
      break;
    case KEY_C:
      TCA_ADD = MULTIPLEXER_C;
      I2C_NUM = I2C_PIN_C;
      break;
    case KEY_D:
      TCA_ADD = MULTIPLEXER_D;
      I2C_NUM = I2C_PIN_D;
      break;
    case KEY_E:
      TCA_ADD = MULTIPLEXER_E;
      I2C_NUM = I2C_PIN_E;
      break;
    case KEY_F:
      TCA_ADD = MULTIPLEXER_F;
      I2C_NUM = I2C_PIN_F;
      break;
    case KEY_G:
      TCA_ADD = MULTIPLEXER_G;
      I2C_NUM = I2C_PIN_G;
      break;
    case KEY_H:
      TCA_ADD = MULTIPLEXER_H;
      I2C_NUM = I2C_PIN_H;
      break;
    case KEY_I:
      TCA_ADD = MULTIPLEXER_I;
      I2C_NUM = I2C_PIN_I;
      break;
    case KEY_J:
      TCA_ADD = MULTIPLEXER_J;
      I2C_NUM = I2C_PIN_J;
      break;
    case KEY_K:
      TCA_ADD = MULTIPLEXER_K;
      I2C_NUM = I2C_PIN_K;
      break;
    case KEY_L:
      TCA_ADD = MULTIPLEXER_L;
      I2C_NUM = I2C_PIN_L;
      break;
    case KEY_M:
      TCA_ADD = MULTIPLEXER_M;
      I2C_NUM = I2C_PIN_M;
      break;
    case KEY_N:
      TCA_ADD = MULTIPLEXER_N;
      I2C_NUM = I2C_PIN_N;
      break;
    case KEY_O:
      TCA_ADD = MULTIPLEXER_O;
      I2C_NUM = I2C_PIN_O;
      break;
    case KEY_P:
      TCA_ADD = MULTIPLEXER_P;
      I2C_NUM = I2C_PIN_P;
      break;
    case KEY_Q:
      TCA_ADD = MULTIPLEXER_Q;
      I2C_NUM = I2C_PIN_Q;
      break;
    case KEY_R:
      TCA_ADD = MULTIPLEXER_R;
      I2C_NUM = I2C_PIN_R;
      break;
    case KEY_S:
      TCA_ADD = MULTIPLEXER_S;
      I2C_NUM = I2C_PIN_S;
      break;
    case KEY_T:
      TCA_ADD = MULTIPLEXER_T;
      I2C_NUM = I2C_PIN_T;
      break;
    case KEY_U:
      TCA_ADD = MULTIPLEXER_U;
      I2C_NUM = I2C_PIN_U;
      break;
    case KEY_V:
      TCA_ADD = MULTIPLEXER_V;
      I2C_NUM = I2C_PIN_V;
      break;
    case KEY_W:
      TCA_ADD = MULTIPLEXER_W;
      I2C_NUM = I2C_PIN_W;
      break;
    case KEY_X:
      TCA_ADD = MULTIPLEXER_X;
      I2C_NUM = I2C_PIN_X;
      break;
    case KEY_Y:
      TCA_ADD = MULTIPLEXER_Y;
      I2C_NUM = I2C_PIN_Y;
      break;
    case KEY_Z:
      TCA_ADD = MULTIPLEXER_Z;
      I2C_NUM = I2C_PIN_Z;
      break;
    case KEY_DELETE:
      TCA_ADD = MULTIPLEXER_DELETE;
      I2C_NUM = I2C_PIN_DELETE;
      break;
    case KEY_FN:
      TCA_ADD = MULTIPLEXER_FN;
      I2C_NUM = I2C_PIN_FN;
      break;    
    default: 
      break;
    }

    Wire.end();
    pinMode(A0, OUTPUT);
    digitalWrite(A0, LOW);
    pinMode(A0, INPUT);
    
    Wire.begin();
    Serial.begin(115200);
    Serial.setTimeout(10);
    tcaselect(I2C_NUM);
    cap.begin(0x5A);
  }
  
  /*Print filtered data*/
  Serial.print(cap.filteredData(0));
  Serial.print("_");
  Serial.print(cap.filteredData(1));
  Serial.print("_");
  Serial.print(cap.filteredData(2));
  Serial.print("_");
  Serial.print(cap.filteredData(3));
  Serial.print("_");
  Serial.print(cap.filteredData(4));
  Serial.print("_");
  Serial.print(cap.filteredData(5));
  Serial.print("_");
  Serial.print(cap.filteredData(6));
  Serial.print("_");
  Serial.print(cap.filteredData(7));
  Serial.print("_");
  Serial.print(cap.filteredData(8));
  Serial.println();
}


