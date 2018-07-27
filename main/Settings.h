#ifndef Settings_h
#define Settings_h

struct StoreStruct {
  char version_byte0,version_byte1,version_byte2,version_byte3; // TODO: convert to char[4] or char*
  uint8_t nodeId;
  bool isSpecial;
};

extern StoreStruct my_settings;

void loadConfig();
void saveConfig();
uint8_t getNodeId();


#endif  // end Settings_h
