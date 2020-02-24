// BIDS.h
// Base : stop-pattern
// v202 Support : TetsuOtter

#ifndef _BIDS_
#define _BIDS_

class c_BIDS {
 public:
  /* ----- constructor ----- */
  c_BIDS();
  c_BIDS(int);
  c_BIDS(int, int);

  /* ----- setter ----- */
  // Train Operation Instruction
  int setReverser(int);
  int setController(int);
  int setController(int, int);
  // keys
  int setBVEKeyPush(int);
  int setBVEKeyRelease(int);
  int setKeyboardDown(int);
  int setKeyboardUp(int);

  /* ----- getter ----- */
  // Vehicle Specification
  int getBrakeNotches();
  int getPowerNotches();
  int getAtsNotch();
  int getB67Notch();
  int getCarNumber();
  // State Quantity of Vehicle
  double getLocation();
  float getSpeed();
  int getTime();
  float getBcPressure();
  float getMrPressure();
  float getErPressure();
  float getBpPressure();
  float getSapPressure();
  float getCurrent();
  // Train Operation Instruction
  int getBrake();
  int getPower();
  int getReverser();
  int getConstantSpeed();
  // Panel
  int getPanel(int);
  // Sound
  int getSound(int);
  // Door
  int getDoor();

  /* ----- definition ----- */
  enum Reverser : int;  // for TRR
  enum key : int;       // for TRK
  enum Car : int;       // for TRIC
  enum E : int;         // for TRIE (tentative)
  enum Handle : int;    // for TRIH
                        //enum Error : int;     // for TRE

 private:
  /* ----- getData ----- */
  auto SerialGet(String);

  /* ----- DataGet ----- */
  auto DataGet(String, String, int);
  auto DataGet(String, int);
  String DataGet(double &, String);

  /* ----- getData ----- */
  int Version;
  int VersionCheck(int, int);
};

#endif  //_BIDS_
