#ifndef Button_h
#define Button_h

class Button
{
  private:
    int my_pin;
    bool debounced_state;
    bool last_state;
    unsigned long last_state_change;
    
  public:
    Button( int );
    bool isOn();
    void loop();
};

#endif
