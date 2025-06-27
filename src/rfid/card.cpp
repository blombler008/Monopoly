#include "card.hpp"

Card::Card(const char* uid, const char* name, int32_t balance) {
    setUID(uid); // Set the UID using the setUID method to avoid code duplication
    setName(name); // We can reuse the setName method to avoid code duplication
    setBalance(balance); // We can reuse the setBalance method to avoid code duplication
}

Card::Card(const Card& card) {  
    const char* uidPtr = card.getUID(); // Get the UID pointer
    log_d("Card::Card(const Card& card) - UID: %p", uidPtr); // Log the UID pointer
    log_d("Card::Card(const Card& card) - UID: %s", uidPtr); // Log the UID pointer

    //setUID(card.getUID()); // Set the UID using the setUID method to avoid code duplication
    setUID(uidPtr); // Set the UID using the setUID method to avoid code duplication
    setName(card.getName()); // We can reuse the setName method to avoid code duplication
    setBalance(card.getBalance()); // We can reuse the setBalance method to avoid code duplication
    setNumber(card.getNumber()); // We can reuse the setNumber method to avoid code duplication 
}

Card::~Card() {
    delete[] uid;
    delete[] name;
}

bool Card::operator==(const Card *card) {
    return operator==(*card); // Check if the memory addresses are the same
}

Card& Card::operator=(const Card& other) {
    if (this != &other) { 
        setUID(other.getUID()); // Use setUID to copy the UID
        setName(other.getName()); // Use setName to copy the name
        setBalance(other.getBalance()); // Use setBalance to copy the balance
        setNumber(other.getNumber()); // Use setNumber to copy the card number 
    }
    return *this;
}

bool Card::operator==(const Card &card) {
    const char* uid  = card.getUID(); 
    int number       = card.getNumber();
    const char* name = card.getName();
    float balance    = card.getBalance();

    const char* thisUid = getUID();
    int thisNumber = getNumber();
    const char* thisName = getName();
    float thisBalance = getBalance();
 
    if (strcmp(uid, thisUid) != 0) {
        return false;
    }
 
    if (number != thisNumber) {
        return false;
    }

    if (strcmp(name, thisName) != 0) {
        return false;
    }

    if (balance != thisBalance) {
        return false;
    }

    return true; 
} 

char *Card::getUID() const {
    return uid;
}

int Card::getNumber() const {
    return number;
}

char* Card::getName() const {
    return name;
}

int32_t Card::getBalance() const {
    return balance;
}

void Card::setBalance(int32_t balance) {
    this->balance = balance;
}

void Card::setUID(const char *uid) { 
    delete[] this->uid; // Free the existing memory
    this->uid = new char[strlen(uid) + 1];
    strcpy(this->uid, uid);
}

void Card::setNumber(int number) {
    this->number = number;
}

void Card::setName(const char *name) { 
    delete[] this->name; // Free the existing memory
    this->name = new char[strlen(name) + 1];
    strcpy(this->name, name); 
} 