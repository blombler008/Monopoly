#include "card.hpp"

Card::Card(const char* uid, int number, const char* name, float balance) {
    this->uid = new char[strlen(uid) + 1]; 
    strcpy(this->uid, uid); 
    setName(name); // We can reuse the setName method to avoid code duplication
    setBalance(balance); // We can reuse the setBalance method to avoid code duplication
    this->number = number; 
}

char* Card::getUID() const {
    return uid;
}

int Card::getNumber() const {
    return number;
}

char* Card::getName() const {
    return name;
}

float Card::getBalance() const {
    return balance;
}

void Card::setBalance(float balance) {
    this->balance = balance;
}

void Card::setName(const char* name) {
    delete[] this->name;
    this->name = new char[strlen(name) + 1];
    strcpy(this->name, name);
}

CardCollection::CardCollection(int initial_size) {
    if (initial_size < 6) {
        initial_size = 6;
    }
    card_collection = new Card[initial_size];
    card_count = 0;
    next_card_number = 1;
    current_max_cards = initial_size;
}

CardCollection::~CardCollection() {
    delete[] card_collection;
}

int CardCollection::save_card(Card* card) {
    if (card_count < current_max_cards) { // Check if the card collection is not full
        card->setNumber(next_card_number++); // Set the card number and increment the counter 
        card_collection[card_count] = *card; // Add the card to the collection
        card_count++; // Increment the card count
        return card->getNumber(); // Return the card number
    } else {
        log_e("Card collection is full");
        return -1; // Indicate an error
    }
}

bool CardCollection::delete_card(int card_number) {
    for (size_t i = 0; i < card_count; ++i) {
        if (card_collection[i].getNumber() == card_number) {
            // Shift remaining cards to fill the gap
            for (size_t j = i; j < card_count - 1; ++j) {
                card_collection[j] = card_collection[j + 1];
            }
            card_count--;
            return true; // Card successfully deleted
        }
    }
    return false; // Card not found
}

Card* CardCollection::find_card_by_number(int card_number) {
    for (size_t i = 0; i < card_count; ++i) {
        if (card_collection[i].getNumber() == card_number) {
            return &card_collection[i];
        }
    }
    return nullptr; // Card not found
}

Card* CardCollection::find_card_by_uid(const char* uid) {
    for (size_t i = 0; i < card_count; ++i) {
        if (strcmp(card_collection[i].getUID(), uid) == 0) {
            return &card_collection[i];
        }
    }
    return nullptr; // Card not found
}

bool CardCollection::update_card_collection_size(int size_change) {
    int new_size = current_max_cards + size_change;
    bool return_value = true;
    if (new_size < 6) {
        log_e("Invalid size change: Minimum size is 6. Setting to 6.");
        new_size = 6; // Minimum size is 6
        return_value = false;
    }

    Card* new_collection = new Card[new_size];
    for (size_t i = 0; i < card_count && i < new_size; ++i) {
        new_collection[i] = card_collection[i];
    }

    delete[] card_collection;
    card_collection = new_collection;
    current_max_cards = new_size;
    return return_value; // Size successfully updated
}