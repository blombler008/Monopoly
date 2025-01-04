#include "keypad.hpp"

byte rows;
byte cols;

byte* rowPins;
byte* colPins;

char* keys; 
bool killKPTask = false;
Keypad* keypad;
 
void keypadloop(void *) { 
    while (1) {
        keypad->getKeys();
        delay(1);

        if(killKPTask) {
            vTaskDelete(NULL);
        }

    } 
}

void keypad_stop() {
    killKPTask = true;
}

void keypad_set_pins(byte* rPins, byte* cPins) {
    rowPins = rPins;
    colPins = cPins; 
}

void keypad_set_row_col_num(const byte r, const byte c){
    rows = r;
    cols = c;
}

void keypad_set_layout(char* layout) {
    keys = makeKeymap(layout);
}

void keypad_setup(void (*listener)(char))  {
    keypad = new Keypad( keys, rowPins, colPins, rows, cols ); 
    keypad->setDebounceTime(20);
    keypad->setHoldTime(500);

    if(listener != NULL) {
        keypad->addEventListener(listener);
        xTaskCreate(keypadloop, "keypad", 10000, NULL, 0, NULL); 
    }
   
    log_i("Keypad Initialised");
    
}

int getKeystateString(char* buf, char key) {
    int idx = keypad->findInList(key); // Find the index of the key in the list
    
    if(idx == -1) { // Check if the key is not found
        strcpy(buf, "NOT_FOUND"); // Copy "NOT_FOUND" to the buffer
        return -1; // Return -1 to indicate the key was not found
    }
    
    KeyState keyS = keypad->key[idx].kstate; // Get the state of the key

    const char* stateStr[] = { "IDLE", "PRESSED", "HOLD", "RELEASED" }; // Define the key state strings
    strcpy(buf, stateStr[keyS]);  // Copy the key state string to the buffer
    return keyS; // Return the key state
}

Keypad getKeypad(void) {
    return *keypad;
}