
#include <Wire.h>

bool isAll = false;
bool flag = false;
const byte DEVICE_ID = 3;
const int ARRAY_SIZE = 200;
 
char finalMessage[ARRAY_SIZE];
unsigned int messageSize = 0;

char text[ARRAY_SIZE];
unsigned int addressWithTextSize = 0;

int idSender = 0;

void createMessage(){
  const char enterSymbol = '\n';
  const char nickname[] = "Arduino #3: ";
  
  finalMessage[0] = text[0]; //адрес
  messageSize++;
  
  for(int k=0; k < sizeof(nickname)/sizeof(*nickname) - 1; k++, messageSize++){
        finalMessage[messageSize] = nickname[k];
    }
  
    for(int k=1; k < addressWithTextSize; k++, messageSize++){
        finalMessage[messageSize] = text[k];
    }
  
  finalMessage[messageSize] = enterSymbol; 
  messageSize++;
  
}

void setup()
{
  Serial.begin(9600);
  Wire.begin(DEVICE_ID);  
    Wire.onReceive(receiveEvent); 
}

void loop()
{
//Получение данных из Serial. Сбор текста сообщения посимвольно:
  if(Serial.available() > 0 ) {
      text[addressWithTextSize] = Serial.read();
      addressWithTextSize++;
      flag = true;
    }
  
  delay(10); 
  
  // когда данные из Serial закончились:
  if(Serial.available() <= 0 && flag == true) { 
      createMessage();
    
    if(finalMessage[0] == '@'){ // сообщение адресовано для всех
      finalMessage[0] = ' ';
      isAll = true;
      Serial.write(finalMessage, messageSize);
    }
    
      if(finalMessage[0] == '1' || isAll){ // начать передачу для DEVICE_ID = 1
      finalMessage[0] = ' ';
          Wire.beginTransmission(1); 
          Wire.write(finalMessage, messageSize); 
      Wire.endTransmission();   
      
      if(!isAll){
          Serial.write(finalMessage, messageSize); 
      }
      
      }

      if(finalMessage[0] == '2' || isAll){ // начать передачу для DEVICE_ID = 2
      finalMessage[0] = ' ';
      Wire.beginTransmission(2);
      Wire.write(finalMessage, messageSize);
      Wire.endTransmission();   
      
      if(!isAll){
          Serial.write(finalMessage, messageSize); 
      }
      
    }
    
    if(finalMessage[0] == '$' && idSender != 0){ // сообщение адресовано для последнего написавшего
      finalMessage[0] = ' ';
      Wire.beginTransmission(idSender);
      Wire.write(finalMessage, messageSize);
      Wire.endTransmission();   
      
      if(!isAll){
          Serial.write(finalMessage, messageSize); 
      }
    }

      messageSize=0;
    addressWithTextSize = 0;
      flag = false;
    isAll = false;
  }
}

// фун-я для обработки сигнала, пришедшего на данный девайс. Прием сообщений:
void receiveEvent(int howMany){
  char receiveText[ARRAY_SIZE];
  unsigned int receiveSize = 0;
  char symbol;
  
  while(Wire.available() > 0){
    symbol = Wire.read(); //считываем из шины
    Serial.write(symbol); //отправляем в Serial то, что считали
    
    receiveText[receiveSize] = symbol;
    receiveSize++;
  }
  
  lastSender(receiveText, receiveSize);
}

void lastSender(char recieveTxt[], int recieveSize){
  const int SIZE_ARR = 3;
  const char ID_ARRAY[SIZE_ARR] = {1, 2, 3};
  const char *NICKNAMES[SIZE_ARR] = {"Arduino #1", "Arduino #2", "Arduino #3"};
  char name[30];
  int nameSize = 0;
  
  for(int i=1; i < recieveSize; i++){
        if(recieveTxt[i] == ':'){
      name[nameSize] = '\0';
      break;
    }
    
    name[nameSize] = recieveTxt[i];
    nameSize++;
    }
  
    for (int i = 0; i < SIZE_ARR; i++) { 
    if (strcmp(name, NICKNAMES[i]) == 0){
      idSender = (int)ID_ARRAY[i];
    }
      
    }
}
