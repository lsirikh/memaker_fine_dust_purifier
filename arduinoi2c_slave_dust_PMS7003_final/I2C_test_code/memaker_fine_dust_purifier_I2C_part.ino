#include <Wire.h>  //Wire supports that Memaker board(arduino) is communicating with raspberry pi though i2c.
////////////////////////////Set the address for memaker board(or ardino)/////////////////////
#define SLAVE_ADDRESS 0x04  
/////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  //////////////setup for Serial monitor start/////////////
  Serial.begin(9600);
  Serial.println("Serial monitor is ready.......");
  //////////////setup for Serial monitor finish////////////
  // put your setup code here, to run once:
  ///////////initialize i2c as slave////////////////////////  
  Wire.begin(SLAVE_ADDRESS);  
  // define callbacks for i2c communication  
  Wire.onReceive(receiveData);  
  Wire.onRequest(sendData);  
  //////////initializing communication finish//////////////
}

void loop() {
  // put your main code here, to run repeatedly:

}

// callback for received data 
// 라즈베리파이로 부터 받은 I2C 통신 명령을 확인하는 함수
void receiveData(int byteCount) {  
  int value=0;
  int number=0;
  //I2C로 부터 받은 데이터가 있으면, 확인
  while (Wire.available()) {
    //명령코드 수행  
    //byte단위의 데이터가 오기 때문에 낮은 숫자는 별도의 컨버팅 작업없이 쓸 수 있음
    //0000000-1111111(10진수로 128개 정도)단위를 활용할 수 있음
    Serial.print("The number of bytes from Master: ");  
    Serial.println(byteCount); 
    
    number = Wire.read();  
    value=Wire.read();
    Serial.print("data received: ");  
    Serial.println(number);  
    Serial.print("data received: ");  
    Serial.println(value);  
//    //number가 4이면 AUTO 모드 ON/OFF하는 명령코드가 됨
//    //이 명령코드는 NUBISON에서 원격으로 데이터 콘트롤하는 부분과 서로 프로토콜을 맞춰줌
//    //사전에 타이젠 코드에서 조건에 맞춰서 명령코드를 설정
//    if (number == 4) {  
//      isAuto=value;
//    }else if(number ==3){
//      //number가 3이면, FAN의 속도를 변화시켜주는 명령으로, value값에 따라 fanSpeed가 바뀜
//      //fanSpeed를 변경하는 명령이 오기 위해서는 타이젠에서 받은 데이터를 통해 현재 이 보드가 
//      //MANUAL 모드라는 것을 인식했을 때만 보낼 수 있도록 프로토콜 설정
//      if(value == 0){
//        fanSpeed = 0;
//      }else if(value ==1){
//        fanSpeed = 50;
//      }else if(value ==2){
//        fanSpeed = 100;
//      }else if(value ==3){
//        fanSpeed = 150;
//      }else if(value ==4){
//        fanSpeed = 200;
//      }else if(value ==5){
//        fanSpeed = 250;
//      }
//    }
  }  
}  
//callback for sending data  
//미메이커보드(아두이노보드)에서 I2C를 통해서 타이젠으로 데이터를 송신하는 함수
void sendData() {
  //int형태의 데이터 타입을 byte타입의 Low와 High로 쪼개서 송신하고
  //수신하는 타이젠측에서 두 데이터를 합성하여 int로 다시 활용하게 됨
  uint16_t a=1000;
  uint16_t b=2000;
  uint16_t c=3000;
  uint16_t d=4000;
  uint16_t e=5000;

  Wire.write(lowByte(a)); // data 전송
  Wire.write(highByte(a)); // data 전송
  Wire.write(lowByte(b)); // data 전송
  Wire.write(highByte(b)); // data 전송
  Wire.write(lowByte(c)); // data 전송
  Wire.write(highByte(c)); // data 전송
  Wire.write(lowByte(d)); // data 전송
  Wire.write(highByte(d)); // data 전송
  Wire.write(lowByte(e)); // data 전송
  Wire.write(highByte(e)); // data 전송
  
//  Wire.write(lowByte(PM03_10)); // data 전송
//  Wire.write(highByte(PM03_10)); // data 전송
//  Wire.write(lowByte(PM10_25)); // data 전송
//  Wire.write(highByte(PM10_25)); // data 전송
//  Wire.write(lowByte(PM25_)); // data 전송
//  Wire.write(highByte(PM25_)); // data 전송
//  Wire.write(lowByte(fanSpeed)); // data 전송
//  Wire.write(highByte(fanSpeed)); // data 전송
//  Wire.write(lowByte(isAuto)); // data 전송
//  Wire.write(highByte(isAuto)); // data 전송
}  
