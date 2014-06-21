#define UpKey 'W'
#define LeftKey 'A'
#define RightKey 'D'
#define DownKey 'S'
#define StartKey 'R'
#define SelectKey 'F'
#define MouseKey MOUSE_LEFT

void setup() {
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Keyboard.begin();
  Mouse.begin();
  Serial1.begin(9600);
  establishContact();
}

void loop() {
  static boolean PrevUp = false;
  static boolean PrevLeft = false;
  static boolean PrevRight = false;
  static boolean PrevDown = false;
  static boolean PrevStart = false;
  static boolean PrevSelect = false;
  static boolean PrevMouse = false;
  static byte StringState = 0;
  static byte StringIndex = 0;
  static char Buffer = 0;
  static char Frame[25];
  static signed char XAxis = 0;
  static signed char YAxis = 0;
  
  while (Serial1.available()) {
    Buffer = Serial1.read();
    if (Buffer == '?') establishContact();
    else {
      if (Buffer == '[') {
        digitalWrite(9, HIGH);
        StringState = 1;
        StringIndex = 0;
      }
      else {
        if (StringState == 1) {
          digitalWrite(9, LOW);
          if (Buffer == ']') {
            StringState = 3;
          }
          else Frame[StringIndex++] = Buffer;
        }
      }
    }
  }
  if (StringState == 3) {
    digitalWrite(8, HIGH);
    StringState = 5;
    
    if ((Frame[0] == 'u') && (PrevUp)) {
      Keyboard.release(UpKey);
      PrevUp = false;
    }
    else if ((Frame[0] == 'U') && (!PrevUp)) {
      Keyboard.press(UpKey);
      PrevUp = true;
    }
    if ((Frame[2] == 'l') && (PrevLeft)) {
      Keyboard.release(LeftKey);
      PrevLeft = false;
    }
    else if ((Frame[2] == 'L') && (!PrevLeft)) {
      Keyboard.press(LeftKey);
      PrevLeft = true;
    }
    if ((Frame[4] == 'r') && (PrevRight)) {
      Keyboard.release(RightKey);
      PrevRight = false;
    }
    else if ((Frame[4] == 'R') && (!PrevRight)) {
      Keyboard.press(RightKey);
      PrevRight = true;
    }
    if ((Frame[6] == 'd') && (PrevDown)) {
      Keyboard.release(DownKey);
      PrevDown = false;
    }
    else if ((Frame[6] == 'D') && (!PrevDown)) {
      Keyboard.press(DownKey);
      PrevDown = true;
    }
    if ((Frame[8] == 'a') && (PrevStart)) {
      Keyboard.release(StartKey);
      PrevStart = false;
    }
    else if ((Frame[8] == 'A') && (!PrevStart)) {
      Keyboard.press(StartKey);
      PrevStart = true;
    }
    if ((Frame[10] == 'b') && (PrevSelect)) {
      Keyboard.release(SelectKey);
      PrevSelect = false;
    }
    else if ((Frame[10] == 'B') && (!PrevSelect)) {
      Keyboard.press(SelectKey);
      PrevSelect = true;
    }
    if ((Frame[12] == 'm') && (PrevMouse)) {
      Mouse.release(MouseKey);
      PrevMouse = false;
    }
    else if ((Frame[12] == 'M') && (!PrevMouse)) {
      Mouse.press(MouseKey);
      PrevMouse = true;
    }
    XAxis = Frame[14] - 73;
    YAxis = Frame[16] - 72;
    Mouse.move(XAxis, YAxis, 0);
    StringState = 0;
    digitalWrite(8, LOW);
  }
}

void establishContact() {
  boolean Sync = false;
  char Buffer = 0;
  
  digitalWrite(13, LOW);
  while (!Sync) {
    if (Serial1.available() >= 2) {
      Buffer = Serial1.read();
      if ((Buffer == '?') && (Serial1.read() == '?')) {
        Sync = true;
        Serial1.write('!');
        digitalWrite(13, HIGH);
      }
    }
  }
}

/*
## Send frame ##

'['
'u' | 'U'
','
'l' | 'L'
','
'r' | 'R'
','
'd' | 'D'
','
'a' | 'A'
','
'b' | 'B'
','
'm' | 'M'
','
'@' - 'R'
','
'@' - 'R'
']'
0x0D
0x0A

*/
