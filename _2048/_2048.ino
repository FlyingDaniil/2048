//i2c oled 0.96
//arduino nano
//5 push buttons (4 controls + reset)

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define UP_BUTTON 6 
#define DOWN_BUTTON 7
#define LEFT_BUTTON 8
#define RIGHT_BUTTON 9

#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(-1);

int matrix[4][4]= {
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0}
};

int SCORE = 0;


void setup() {
  
  Serial.begin(9600);
  delay(2000);
  Serial.println("2048 by Daniil Zhuk");

  pinMode(UP_BUTTON, INPUT);
  pinMode(DOWN_BUTTON, INPUT);
  pinMode(LEFT_BUTTON, INPUT);
  pinMode(RIGHT_BUTTON, INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  
  StartGame();
  PrintToSerial();
  PrintToDisplay();
  
}

void loop() {
  
  // 1 down
  // 2 right
  // 3 up
  // 4 left
  
  //snachala stroka potom element

//  //serial
//  if(Serial.available()>0){
//    
//    int action = Serial.read();
//    Serial.println(action);
//
//    //1
//    if (action == 49){
//      MoveDown();
//    }  
//
//    //2
//    if (action == 50){
//      MoveRight();
//    }
//
//    //3
//    if (action == 51){
//      MoveUp();      
//    }
//
//    //4
//    if (action == 52){
//      MoveLeft();
//    }
//    
//    AfterMove();
//  }

  if (digitalRead(UP_BUTTON)==HIGH){
    MoveUp();
    AfterMove();
  }

  if (digitalRead(DOWN_BUTTON)==HIGH){
    MoveDown();
    AfterMove();
  }

  if (digitalRead(LEFT_BUTTON)==HIGH){
    MoveLeft();
    AfterMove();
  }

  if (digitalRead(RIGHT_BUTTON)==HIGH){
    MoveRight();
    AfterMove();
  }

  

  //buttons
}


void StartGame(){
  matrix[random(0,2)][random(0,4)]=2;
  //matrix[random(2,4)][random(0,4)]=2;
}


void PrintToSerial(){
  
  for (int i = 0; i<4; i++){
    for (int j = 0; j<3; j++){
      Serial.print(matrix[i][j]);Serial.print("\t");
    }
    Serial.println(matrix[i][3]);
  }
}


void LostCheck(){
  
  //check zeros
  bool lost = true;
  
  for (int i = 0;i<4;i++){
    for (int j = 0;j<4;j++){
      if (matrix[i][j]==0){
        lost = false;
      }
    }
  }

  if (lost == false){
    RandomElement();
  }
  
  //check twins
  for (int i = 0;i<3;i++){
    for (int j = 0;j<4;j++){
      if (matrix[i][j]==matrix[i+1][j]){
        lost = false;
      }
    }
  }
  
  for (int i = 0;i<4;i++){
    for (int j = 0;j<3;j++){
      if (matrix[i][j]==matrix[i][j+1]){
        lost = false;
      }
    }
  }
  
  if (lost){
    Serial.println("GAMEOVER");

    display.clearDisplay();

    display.setTextSize(2);
    display.setCursor(0,0);
    display.println("GAMEOVER");

    display.display();
    
    //ClearMatrix();
    SCORE = 0;
  } else{
    PrintToDisplay();
  }
}


void ClearMatrix(){
  for (int i = 0;i<4;i++){
    for (int j = 0;j<4;j++){
      matrix[i][j]=0;
    }
  }
}


void WinCheck(){

  //win check
  bool win = false;
  for (int i = 0;i<4;i++){
    for (int j = 0;j<4;j++){
      if (matrix[i][j]>2000){
        win = true;
      }
    }
  }

  if (win){
    Serial.println("You Won!");
    Serial.print("Score: ");
    Serial.println(SCORE);

    display.clearDisplay();

    display.setTextSize(2);
    display.setCursor(0,0);
    display.println("YOU WON");

    display.display();
    
  } else {
    PrintToDisplay();
  }

  //RandomElement();
}


void RandomElement(){
  
  //random "2" in cell
  bool randomed = false;
  
  while (randomed == false){
    
    int x = random(0,4);
    int y = random(0,4);
    
    if (matrix[x][y] == 0){
      matrix[x][y] = 2;
      randomed = true;
    }
    
  }
  
}


void PrintToDisplay(){
  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(0,0);
  display.println(SCORE);

  display.setTextSize(1);
  
  for (int i = 0; i<4; i++){
    for (int j = 0; j<4; j++){
      display.setCursor(32*j,23+10*i);
      if (matrix[i][j]!=0){
        display.print(matrix[i][j]);
      }else{
        display.print(".");
      }
      
    }
  }
  
  display.display();
  
}


void MoveDown(){
  for (int k = 0; k<4;k++){
    for (int i = 3;i>0;i--){
      for (int j = 0; j<4;j++){
        if (matrix[i][j]==0){
          matrix[i][j]=matrix[i-1][j];
          matrix[i-1][j]=0;
        }
      }
    }
  }
  
  for (int i = 3;i>0;i--){
    for (int j = 0; j<4;j++){
      if (matrix[i][j]!=0 && matrix[i-1][j]==matrix[i][j]){
        matrix[i][j]=matrix[i][j]+matrix[i-1][j];
        SCORE=SCORE+matrix[i][j];
        matrix[i-1][j]=0;
      }
    }
  }
  
  for (int k = 0; k<4;k++){
    for (int i = 3;i>0;i--){
      for (int j = 0; j<4;j++){
        if (matrix[i][j]==0){
          matrix[i][j]=matrix[i-1][j];
          matrix[i-1][j]=0;
        }
      }
    }
  }
}


void MoveRight(){
  for (int k = 0; k<4;k++){
    for (int i = 3; i>0; i--){
      for (int j = 0; j<4; j++){
        if (matrix[j][i]==0){
          matrix[j][i]=matrix[j][i-1];
          matrix[j][i-1]=0;
        }
      }
    }
  }
  
  for (int i = 3; i>=0; i--){
    for (int j = 3; j>0; j--){
      if (matrix[i][j]!=0 && matrix[i][j-1]==matrix[i][j]){
        matrix[i][j]=matrix[i][j]+matrix[i][j-1];
        SCORE=SCORE+matrix[i][j];
        matrix[i][j-1]=0;
      }
    }
  }
  
  for (int k = 0; k<4;k++){
    for (int i = 3; i>0; i--){
      for (int j = 0; j<4; j++){
        if (matrix[j][i]==0){
          //Serial.print("!");
          matrix[j][i]=matrix[j][i-1];
          matrix[j][i-1]=0;
        }
      }
    }
  }
}


void MoveUp(){
  for (int k = 0; k<4;k++){
    for (int i = 0;i<3;i++){
      for (int j = 0; j<4;j++){
        if (matrix[i][j]==0){
          //Serial.print("*");
          matrix[i][j]=matrix[i+1][j];
          matrix[i+1][j]=0;
        }
      }
    }
  }
  
  for (int i = 0;i<3;i++){
    for (int j = 0; j<4;j++){
      if (matrix[i][j]!=0 && matrix[i+1][j]==matrix[i][j]){
        matrix[i][j]=matrix[i][j]+matrix[i+1][j];
        SCORE=SCORE+matrix[i][j];
        matrix[i+1][j]=0;
      }
    }
  }
  
  for (int k = 0; k<4;k++){
    for (int i = 0;i<3;i++){
      for (int j = 0; j<4;j++){
        if (matrix[i][j]==0){
          matrix[i][j]=matrix[i+1][j];
          matrix[i+1][j]=0;
        }
      }
    }
  }
}


void MoveLeft(){
  for (int k = 0; k<4;k++){
    for (int j = 0; j<3; j++){
      for (int i = 0; i<4; i++){
        if (matrix[i][j]==0){
          matrix[i][j]=matrix[i][j+1];
          matrix[i][j+1]=0;
        }
      }
    }
  }
 
  for (int j = 0; j<3; j++){
    for (int i = 0; i<4; i++){
      if (matrix[i][j]!=0 && matrix[i][j+1]==matrix[i][j]){
        matrix[i][j]=matrix[i][j]+matrix[i][j+1];
        SCORE=SCORE+matrix[i][j];
        matrix[i][j+1]=0;
      }
    }
  }
  
  for (int k = 0; k<4;k++){
    for (int j = 0; j<3; j++){
      for (int i = 0; i<4; i++){
        if (matrix[i][j]==0){
          matrix[i][j]=matrix[i][j+1];
          matrix[i][j+1]=0;
        }
      }
    }
  }
}


void AfterMove(){
  PrintToSerial();
  //PrintToDisplay();
  LostCheck();
  WinCheck();
  
  //RandomElement();
}

