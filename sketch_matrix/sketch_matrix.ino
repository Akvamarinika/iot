#define PERIOD 50
uint32_t timer = 0;
static char prev_symbol = 0;
//const int N[] = {9, 8, 7, 6};    //строки
//const int M[] = {5, 4, 3, 2};   //столбцы
const int N[] = {6, 7, 8, 9};
const int M[] = {2, 3, 4, 5};
const char keyboard [4][4] = {      
  {'1', '2', '3', '+'},
  {'4', '5', '6', '-'},
  {'7', '8', '9', '*'},
  {'0', 'C', '=', '/'}
};


long num1,num2;
double result;
char operation, symb;


char getSymbol() {
  char symbol = 0;
  if (millis() - timer >= PERIOD) {return 0;} 
  else{
    for (byte n = 0; n <= 3; n++) {    
        digitalWrite(N[n], LOW);
      for (byte m = 0; m <= 3; m++) {  
        if (!digitalRead(M[m])) {
          symbol = keyboard[n][m];             
        }
      }
      digitalWrite(N[n], HIGH);       
    }
 }
  timer += PERIOD;   
    
  if (symbol == prev_symbol) {return 0;}
  else{ prev_symbol = symbol;}
  
  return symbol;
}


double calc(long num1, char operation, long num2) {
  switch(operation){
    case '-':
    result = num1-num2;
    break;
    case '+':
    result = num1+num2;
    break;
    case '*':
    result = num1*num2;
    break;
    case '/':
    result = num1/num2;
    break;
 }
 return result;
}

void getNum(long &num, char symb){
     if (symb >='0' && symb <='9'){
        num = num*10 + (symb -'0'); 
       Serial.println("num:");
       Serial.println(num);
      }

}

void cls(long &num1, long &num2, double &result, char &operation){
  num1=0;
  num2=0;
  result=0;
  operation=0;
}

void getInput(){
  while(true){
     symb = getSymbol();
     if (symb == 'C'){
        cls(num1, num2, result, operation);
      }
        getNum(num1, symb);
     if (num1 !=0 && (symb=='-' || symb=='+' || symb=='*' || symb=='/')) {
        operation = symb;
        Serial.print("operation: ");
        Serial.println(operation);
        break;
      }
  }
    while(true){
      if (symb == 'C'){
        break;
      }
     symb = getSymbol();
     if (symb == 'C'){
        cls(num1, num2, result, operation);
      }
        getNum(num2, symb);
     if (symb == '=' && num2 !=0) {
        result = calc(num1, operation, num2);
        Serial.println("result: ");
        Serial.print(num1);
        Serial.print("  ");
        Serial.print(operation);
        Serial.print("  ");
        Serial.print(num2);
        Serial.print(" = ");
        Serial.println(result);
        break;
      }
  }
}



void setup() {
  for (int i = 0; i <= 3; i++) { 
    pinMode(N[i], OUTPUT);        //строки
    pinMode(M[i], INPUT_PULLUP);  // столбцы
    digitalWrite(N[i], HIGH);
  }
  Serial.begin(9600);
  Serial.println("port is ready...");
}

void loop() {
 /* char symbol = getSymbol();             
  if (symbol) {                     
    Serial.print(symbol);
  }*/

  getInput();
}
