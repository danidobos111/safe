/*------------------------------------------
               KEYCOMBINATIONS:

MENU:
A: sign in
B: sign up

LOGIN:
#: ENTER
*: delete
* (when it's empty): go back to MENU

ADMIN PANEL:
1: go left in the list
2: execute command
3: go right in the list

--------------------------------------------*/

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Keypad.h>
#include <vector>
#include <utility>
using namespace std;

#define step 42
#define dir 41

LiquidCrystal_I2C lcd(0x27,  16, 2);

vector<pair<String, String>> users = {
  {"A","12345"},
  {"B","12345"},
  {"C","12345"},
  {"D","12345"},
  {"ABBA","123"},
  {"ABC","123"},
  {"ABCD","123"}
};

vector<String> admins = {
  "A", "B", "C", "D"
};

//----------------KEYPAD SETUP------------------------
const int ROWS = 4;
const int COLS = 4;

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

uint8_t rowpins[ROWS] = {4,5,6,7};
uint8_t colpins[COLS] = {15,16,17,18};

Keypad keypad = Keypad(makeKeymap(keys), rowpins, colpins, ROWS, COLS);
//---------------------------------------------------

uint8_t leftArrow[8] = {
  B00010, B00110, B01110, B11110, B01110, B00110, B00010, B00000
};

uint8_t rightArrow[8] = {
  B01000, B01100, B01110, B01111, B01110, B01100, B01000, B00000
};

bool adminLogout = false; // ezzel jelezzük az admin_UI-nak, hogy ki kell lépni

void setup() {
  pinMode(step, OUTPUT);
  pinMode(dir, OUTPUT);
  Wire.begin(1, 2);
  lcd.init();
  lcd.backlight();

  Serial.begin(9600);

  lcd.setCursor(0,0);
  lcd.print("A to sign in");
  lcd.setCursor(0,1);
  lcd.print("B to sign up");

  lcd.createChar(0, leftArrow);
  lcd.createChar(1, rightArrow);
}

void open_safe(){
  digitalWrite(dir, HIGH);
  for(int i = 0;i<200;i++){
    digitalWrite(step, HIGH);
    delay(1);
    digitalWrite(step, LOW);
    delay(1);
  }
}

void close_safe(){
  digitalWrite(dir, LOW);
  for(int i = 0;i<200;i++){
    digitalWrite(step, HIGH);
    delay(1);
    digitalWrite(step, LOW);
    delay(1);
  }
}

// -------- admin funkciók --------

void signup();

void create_new_acc() {
  signup();
  lcd.clear();
}

void change_password() {
  lcd.clear();
  lcd.print("Username:");
  String textin = "";
  int idx = -1;

  while(true){
    char key = keypad.getKey();
    if (key != NO_KEY) {
      if (key == '#') {
        if (idx == -1) {
          // username keresése
          for (int i = 0; i < users.size(); i++) {
            if (users[i].first == textin) {
              idx = i;
              break;
            }
          }
          if (idx != -1) {
            lcd.clear();
            lcd.print("New password:");
            textin = "";
          } else {
            lcd.clear();
            lcd.print("Unknown user!");
            delay(1500);
            lcd.clear();
            lcd.print("Username:");
            textin = "";
          }
        } else {
          // új jelszó mentése
          if (textin != "") {
            users[idx].second = textin;
            lcd.clear();
            lcd.print("Password changed!");
            delay(1500);
            return;
          } else {
            lcd.clear();
            lcd.print("Empty!");
            delay(1500);
            lcd.clear();
            lcd.print("New password:");
          }
        }
      }
      else if (key == '*') {
        textin = "";
        lcd.clear();
        if (idx != -1) lcd.print("New password:");
        else lcd.print("Username:");
      }
      else {
        if (textin.length() < 16) textin += key;
        lcd.setCursor(0, 1);
        lcd.print(textin);
      }
    }
  }
}

void change_status() {
  lcd.clear();
  lcd.print("Username:");
  String textin = "";

  while(true){
    char key = keypad.getKey();
    if (key != NO_KEY) {
      if (key == '#') {
        bool found = false;
        for (int i = 0; i < users.size(); i++) {
          if (users[i].first == textin) { found = true; break; }
        }

        if (!found) {
          lcd.clear();
          lcd.print("Unknown user!");
          delay(1500);
          lcd.clear();
          lcd.print("Username:");
          textin = "";
          continue;
        }

        // admin státusz keresése/váltása
        int adminIdx = -1;
        for (int i = 0; i < admins.size(); i++) {
          if (admins[i] == textin) { adminIdx = i; break; }
        }

        if (adminIdx != -1) {
          admins.erase(admins.begin() + adminIdx);
          lcd.clear();
          lcd.print("Admin removed");
        } else {
          admins.push_back(textin);
          lcd.clear();
          lcd.print("Admin added");
        }
        delay(1500);
        return;
      }
      else if (key == '*') {
        textin = "";
        lcd.clear();
        lcd.print("Username:");
      }
      else {
        if (textin.length() < 16) textin += key;
        lcd.setCursor(0, 1);
        lcd.print(textin);
      }
    }
  }
}

void log_out(){
  adminLogout = true;
}

void (*commands[])() = {
  create_new_acc,
  change_password,
  change_status,
  open_safe,
  close_safe,
  log_out
};

String commandNames[] = {
  "create new acc",
  "change password",
  "change status",
  "open safe",
  "close safe",
  "log out"
};

const int commandCount = sizeof(commandNames) / sizeof(commandNames[0]);

int commnum = 0;

void admin_UI(){
  adminLogout = false;
  commnum = 0;
  lcd.clear();

  while(!adminLogout){
    char key = keypad.getKey();
    if(key != NO_KEY){
      switch(key){
        case '1': // előző parancs
          if(commnum > 0) commnum--;
          else commnum = commandCount - 1;
          lcd.clear();
          break;
        case '3': // következő parancs
          if(commnum < commandCount - 1) commnum++;
          else commnum = 0;
          lcd.clear();
          break;
        case '2': // parancs futtatása
          commands[commnum]();
          lcd.clear();
          break;
      }
    }

    lcd.setCursor(0,0);
    lcd.print(commandNames[commnum]);
    lcd.setCursor(0,1);
    lcd.write((uint8_t)0); // bal nyíl
    lcd.setCursor(15,1);
    lcd.write((uint8_t)1); // jobb nyíl
  }

  // kilépéskor visszaáll a főképernyő
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("A to sign in");
  lcd.setCursor(0,1);
  lcd.print("B to sign up");
}

void signin(){
  lcd.clear();
  int x = -1;
  String textin = "";
  bool usern = false;

  lcd.print("Username:");

  while(true){
    char key = keypad.getKey();
    if (key != NO_KEY) {
      if (key == '#') {
        if (!usern) {
          x = -1;
          for (int i = 0; i < users.size(); i++) {
            if (users[i].first == textin) {
              x = i;
              break;
            }
          }
          if (x != -1) {
            usern = true;
            lcd.clear();
            lcd.print("Password:");
          } else {
            lcd.clear();
            lcd.print("Unknown user!");
            delay(1500);
            lcd.clear();
            lcd.print("Username:");
          }
        } else {
          lcd.clear();
          if (textin == users[x].second) {
            lcd.print("Welcome");
            lcd.setCursor(0,1);
            lcd.print(users[x].first);
            delay(1000);

            bool isAdmin = false;
            for (String i : admins) {
              if (users[x].first == i) { isAdmin = true; break; }
            }

            if (isAdmin) {
              admin_UI();
              return;
            } else {
              open_safe();
              delay(2000);
              close_safe();
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("A to sign in");
              lcd.setCursor(0,1);
              lcd.print("B to sign up");
              return;
            }
          } else {
            lcd.print("Wrong password!");
            delay(1500);
          }
          lcd.clear();
          usern = false;
          x = -1;
          lcd.print("Username:");
        }
        textin = "";
      }
      else if (key == '*') {
        if(textin == ""){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("A to sign in");
          lcd.setCursor(0,1);
          lcd.print("B to sign up");
          return;
        }
        textin = "";
        lcd.clear();
        if (x != -1) lcd.print("Password:");
        else lcd.print("Username:");
      }
      else {
        if (textin.length() < 16) textin += key;
        lcd.setCursor(0, 1);
        lcd.print(textin);
      }
    }
  }
}

void signup(){
  lcd.clear();
  lcd.print("Username(max 16)");
  String uname = "";
  String upassw = "";
  bool un = false;

  while(true){
    char key = keypad.getKey();
    if(key != NO_KEY){
      if(key == '#'){
        if(!un){
          bool van = false;
          for (int i = 0; i < users.size(); i++) {
            if (users[i].first == uname) { van = true; break; }
          }

          if (uname == "") {
            lcd.clear();
            lcd.print("Empty");
            delay(1500);
            lcd.clear();
            lcd.print("Username(max 16)");
          }
          else if (van) {
            lcd.clear();
            lcd.print("Already taken");
            delay(1500);
            lcd.clear();
            uname = "";
            lcd.print("Username(max 16)");
          }
          else {
            lcd.clear();
            lcd.print("Good choice");
            delay(1500);
            lcd.clear();
            lcd.print("Password(max 16)");
            un = true;
          }
        }
        else {
          if (upassw == "") {
            lcd.clear();
            lcd.print("Empty");
            delay(1500);
            lcd.clear();
            lcd.print("Password(max 16)");
          } else {
            users.push_back({uname, upassw});
            lcd.clear();
            lcd.print("Account created!");
            delay(1500);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("A to sign in");
            lcd.setCursor(0,1);
            lcd.print("B to sign up");
            return;
          }
        }
      }
      else if (key == '*') {
        lcd.clear();
        if (un) {
          if(upassw == ""){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("A to sign in");
          lcd.setCursor(0,1);
          lcd.print("B to sign up");
          return;
        }
          upassw = "";
          lcd.print("Password(max 16)");
        } else {
          if(uname == ""){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("A to sign in");
          lcd.setCursor(0,1);
          lcd.print("B to sign up");
          return;
        }
          uname = "";
          lcd.print("Username(max 16)");
        }
      }
      else {
        lcd.setCursor(0, 1);
        if(un){
          if (upassw.length() < 16) upassw += key;
          lcd.print(upassw);
        }else{
          if (uname.length() < 16) uname += key;
          lcd.print(uname);
        }
      }
    }
  }
}

void loop() {
  char key = keypad.getKey();
  if(key != NO_KEY){
    switch(key){
      case 'A':
        signin();
        break;
      case 'B':
        signup();
        break;
    }
  }
}