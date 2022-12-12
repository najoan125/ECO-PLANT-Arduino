#include <SoftwareSerial.h>
#include <stdlib.h>
SoftwareSerial btSerial = SoftwareSerial(9,10); //TX, RX 
String byteToString = ""; //입력받은 문자열
int inputValue[3] = {1,2,3}; //적용 데이터
bool commandActive = false;
int inputCount = 0;

bool isInit = false;

int temp = 25;
int hm = 6;


unsigned long time_previous, time_current;
//////////////////////////////////

void setup() {
  Serial.begin(9600);
  btSerial.begin(9600);
  time_previous = millis();
}

void loop() {
  ///////////////////////////////////////////////////////////////////////////////////////////////
 while(btSerial.available()) {//안드로이드 앱에서 아두이노로 데이터 전송
    byte data = btSerial.read();//바이트 단위로 읽힘
    
    //JACKPOT의 J가 들어오면 byteToString 값 초기화
    if(data == 'A' && !isInit){ 
      byteToString = "";
    }

    if(data == 'I' && !isInit){
      byteToString = "";
      isInit = true;
    }

    if (data == 'E'){
      isInit = false;
    }
    
    byteToString += char(data); //char로 형변환 안하면 숫자만 저장됨

    //커맨드 들어옴
    if(byteToString == "APPLY"){
      //Serial.println(byteToString);
      commandActive = true;
      byteToString = "";
      continue;
    }

    if(byteToString == "INITIALIZE"){
      //Serial.println(byteToString);
      int bridge = hm*100+temp+1000;
      char init[3];
      itoa(bridge, init, 10);
      btSerial.write(init);
    }

    //JACKPOT 커맨드 받고 뒤 값 3개 받는 과정
    if(commandActive){
      inputValue[inputCount] = data - 48;//어차피 숫자라 형변환 필요 없음, 아스키코드 형태라서 -48이나 -'0'해야함
      inputCount += 1;
    }

    //값 3개 다 받았으면 출력
    if(inputCount == 3){
      inputCount = 0;
      commandActive = false;
      hm = inputValue[0];
      if (inputValue[1] > 0)
        temp = inputValue[1] *10 + inputValue[2];
      else
        temp = inputValue[2];
      //Serial.println(hm);
      //Serial.println(temp);
      int bridge = hm*100+temp+1000;
      char init[3];
      itoa(bridge, init, 10);
      Serial.print(init);
    }
  }
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  time_current = millis();
  if(time_current - time_previous >= 10000){
    time_previous = time_current;
    int light0 = map(analogRead(A0),1023,0,0,100);
    int light1 = map(analogRead(A1),1023,0,0,100);
    char cl0[2];
    char cl1[2];
    int cli0 = 20;
    int cli1 = 21;
    itoa(cli0, cl0, 10);
    itoa(cli1, cl1, 10);
    if (light0 > light1){
      Serial.write(cl0);
    }
    else{
      Serial.write(cl1);
    }
  }
}