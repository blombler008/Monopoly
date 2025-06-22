#include "keypad.hpp"
uint8_t address; // I2C address for the keypad

char keys[20]; 
bool killKPTask = false;
I2CKeyPad* keypad;

KEYPADCallback listener = nullptr; // Callback function for keypad events


void keypadloop(void *) { 
    while (1) {
        if(killKPTask) {
            vTaskDelete(NULL);
        }
        delay(20); // Delay to avoid busy-waiting and allow other tasks to run
        if (!keypad->isPressed()) {
            continue; // Return -1 if no key is pressed
        }
        char ch = keypad->getChar();     // note we want the translated char
        int key = keypad->getLastKey(); 

        if(listener != nullptr)
            listener(ch); // Call the listener callback with the pressed key character

    } 
}

void keypad_stop() {
    killKPTask = true;
}

void keypad_set_address(uint8_t addr) {
    address = addr; // Set the I2C address for the keypad
} 

void keypad_set_layout(const char* layout) {
    strcpy(keys, layout); // Copy the provided layout string to the keys variable 
}

void keypad_setup(void (*KEYPADCallback)(char))  {
    keypad =  new I2CKeyPad(address); // I2C address for the keypad; 
    Wire.begin(I2C_SDA, I2C_SCL); // Initialize the I2C bus with specified SDA and SCL pins
    Wire.setClock(num_to_khz(400)); // Set the I2C clock speed
    keypad->begin(); // Initialize the keypad
    keypad->loadKeyMap(keys); // Load the keymap into the keypad
    
    // Define the keys for the keypad 
    if(listener != NULL) {
        listener = KEYPADCallback; // Set the callback function for keypad events
        // xTaskCreate(keypadloop, "keypad", 10000, NULL, 0, NULL); 
    }
   
    log_i("Keypad Initialised");
    
}
 

I2CKeyPad getKeypad(void) {
    return *keypad;
}