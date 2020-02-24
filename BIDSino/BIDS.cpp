//BIDS.h
//Base : stop-pattern
//v202 Support : TetsuOtter

#include "BIDS.h"
#include <arduino.h>
#define INITIAL_BAUDRATE 19200
#define TARGET_VERSION 202

/* ----- constructor ----- */
c_BIDS::c_BIDS() {
  this->Version = VersionCheck(TARGET_VERSION, INITIAL_BAUDRATE);
}
c_BIDS::c_BIDS(int v = TARGET_VERSION) {
  this->Version = VersionCheck(v, INITIAL_BAUDRATE);
}
c_BIDS::c_BIDS(int v = TARGET_VERSION, int baudRate = INITIAL_BAUDRATE) {
  this->Version = VersionCheck(v, baudRate);
}

/* ----- getData ----- */
auto c_BIDS::SerialGet(String Command) {
  unsigned long sendtime;
  int cnt = 0;
  Serial.println(Command);
  sendtime = millis();
  while (Serial.available() <= 0) {
    //failed to get data (after 1 minute)
    if (cnt >= 6) return 0;

    //retransmission (every 10 seconds)
    if (millis() - sendtime > 10000) {
      Serial.print(Command);
      Serial.print("\n");
      sendtime = millis();
      cnt++;
    }
  }

  float retFlo = 0;
  return (DataGet(retFlo, Command + "X") != "") ? retFlo : 0;
}

/* ----- DataGet ----- */
auto c_BIDS::DataGet(String Identifier, String Symbol, int Num) {
  return SerialGet("TR" + Identifier + Symbol + String(Num));
}
auto c_BIDS::DataGet(String Identifier, int Num) {
  return DataGet(Identifier, "", Num);
}
String c_BIDS::DataGet(float &Data, String Header = "TR") {
  if (Serial.available() <= 0) return -1;
  String GotData = Serial.readStringUntil('\n');
  GotData.replace("\n", "");

  if (GotData.startsWith(Header)) {
    Data = GotData.substring(GotData.indexOf("X")).toFloat();
    return GotData;
  } else {
    Data = 0;
    return "";
  }
}

int c_BIDS::VersionCheck(int VersionNum, int BaudRate) {
  Serial.begin(INITIAL_BAUDRATE);
  int ret, vnum;
  if (VersionNum >= 202 && BaudRate != INITIAL_BAUDRATE) {
    vnum = SerialGet("TRV" + String(VersionNum) + "BR" + String(BaudRate));
    if (vnum >= 202) {
      Serial.end();
      Serial.begin(BaudRate);
    }
  } else
    vnum = DataGet("V", VersionNum);

  ret = vnum < VersionNum ? VersionNum : vnum;
  return ret;
}

/* ----- setter ----- */
// Train Operation Instruction
int setReverser(int pos) {
  if (-1 <= pos && pos <= 1)
    return DataGet("R", pos);
  return 0;
};
int setController(int hand) {
  return DataGet("S", hand);
};
int setController(int power, int brake) {
  if (brake >= 0) {
    int p = DataGet("P", power);
    int b = DataGet("B", brake);
  }
  if (p == 0 && b == 0)
    return 0;
};
// keys
int setBVEKeyPush(int key) {
  return DataGet("K", "P", key);
};
int setBVEKeyRelease(int key) {
  return DataGet("K", "R", key);
};
int setKeyboardDown(int) {
  return DataGet("K", "D", key);
};
int setKeyboardUp(int) {
  return DataGet("K", "U", key);
};

/* ----- getter ----- */
// Vehicle Specification
int getBrakeNotches() {
  return DataGet("I", "C", Car::BrakeNotchCount);
};
int getPowerNotches() {
  return DataGet("I", "C", Car::PowerNotchCount);
};
int getAtsNotch() {
  return DataGet("I", "C", Car::ATSNotchCount);
};
int getB67Notch() {
  return DataGet("I", "C", Car::B67NotchCount);
};
int getCarNumber() {
  return DataGet("I", "C", Car::CarNumber);
};
// State Quantity of Vehicle
double getLocation() {
  return DataGet("I", "E", E::Location);
};
float getSpeed() {
  return DataGet("I", "E", E::Speed);
};
int getTime() {
  return DataGet("I", "E", E::Time);
};
float getBcPressure() {
  return DataGet("I", "E", E::BcPressure);
};
float getMrPressure() {
  return DataGet("I", "E", E::MrPressure);
};
float getErPressure() {
  return DataGet("I", "E", E::ErPressure);
};
float getBpPressure() {
  return DataGet("I", "E", E::BpPressure);
};
float getSapPressure() {
  return DataGet("I", "E", E::SapPressure);
};
float getCurrent() {
  return DataGet("I", "E", E::Current);
};
// Train Operation Instruction
int getBrake() {
  return DataGet("I", "H", Handle::BrakeNotch);
};
int getPower() {
  return DataGet("I", "H", Handle::PowerNotch;
};
int getReverser() {
  return DataGet("I", "H", Handle::Reverser;
};
int getConstantSpeed() {
  return DataGet("I", "H", Handle::ConstantSpeed;
};
// Panel
int getPanel(int index) {
  return DataGet("I", "P", index);
};
// Sound
int getSound(int index) {
  return DataGet("I", "S", index);
};
// Door
int getDoor() {
  return DataGet("I", "D", 0);
};

enum c_BIDS::Reverser : int {
  Rear = -1,    //後
  Neutral = 0,  //切
  Front = 1,    //前
};
enum c_BIDS::key : int {
  Horn_1,                //default:enter
  Horn_2,                //default:+
  MusicHorn,             //default:-
  ConstantSpeedControl,  //default:BackSpace
  S,                     //default:Space
  A_1,                   //default:Insert
  A_2,                   //default:Delete
  B_1,                   //default:Home
  B_2,                   //default:End
  C_1,                   //default:PageUp
  C_2,                   //default:Next
  D,                     //default:2
  E,                     //default:3
  F,                     //default:4
  G,                     //default:5
  H,                     //default:6
  I,                     //default:7
  J,                     //default:8
  K,                     //default:9
  L,                     //default:0
};
enum c_BIDS::Car : int {
  BrakeNotchCount,  //Number of Brake Notches
  PowerNotchCount,  //Number of Power Notches
  ATSNotchCount,    //ATS Cancel Notch
  B67NotchCount,    //80% Brake (67 degree)
  CarNumber,        //Number of Cars
};
enum c_BIDS::E : int {
  Location,     //Train Position (Z-axis) [m]
  Speed,        //Train Speed [km/h]
  Time,         //Time [ms]
  BcPressure,   //Pressure of Brake Cylinder [Pa]
  MrPressure,   //Pressure of MR [Pa]
  ErPressure,   //Pressure of ER [Pa]
  BpPressure,   //Pressure of BP [Pa]
  SapPressure,  //Pressure of SAP [Pa]
  Current,      //Current [A]
};
enum c_BIDS::Handle : int {
  BrakeNotch,     //Brake Notch
  PowerNotch,     //Power Notch
  Reverser,       //Reverser Position
  ConstantSpeed,  //Constant Speed Control
};
/*
enum c_BIDS::Error : String {
  0 = "原因不明エラー",
  1 = "コンバータとBIDSpp.dllとの間の接続が確立されていない",
  2 = "要求情報コードの数値部が不正",
  3 = "要求情報コードの記号部が不正",
  4 = "識別子が不正",
  5 = "数値変換がオーバーフローした",
  6 = "要求情報コードの数値部に数値以外が混入している",
  7 = "要求情報コードの数値部もしくは記号部が不正",
  8 = "BVEのウィンドウハンドルを取得できない(キーイベント送信時)",
  9 = "(情報なし)",
  10 = "(情報なし)",
  11 = "(情報なし)",
};
*/