#ifndef Button_h
#define Button_h

class Button
{
  private:
    int my_pin;
    bool debounced_state;
    bool last_state;
    bool changed_this_loop;
    unsigned long last_state_change;
    
  public:
    Button( int );
    bool isOn();
    void loop();
    bool stateChanged();  // true if state changed since calling loop() last
};

#endif
