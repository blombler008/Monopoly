#include "card.hpp"

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

bool CardCollection::is_uid_unique(const char* uid) {  
    uint8_t card_count = this->card_count; // Get the current number of cards in the collection 
    for (size_t i = 0; i < card_count; ++i) {
        // Check if the UID matches any existing card's UID
        const char* existing_uid = card_collection[i].getUID();
        if (existing_uid == nullptr) continue; // Skip if the UID is null
        // Compare the UIDs
        if (uid == nullptr) continue; // Skip if the UID to check is null
        // If the UIDs match, set result to false
        // Use strcmp to compare the UIDs 
        if (!strcmp(existing_uid, uid)) {
            return false; // UID is not unique
        }
    }
    return true;
}

int CardCollection::save_card(Card* card) { 
    if (!is_uid_unique(card->getUID())) {
        log_e("Card UID is not unique");
        return -1; // Indicate an error
    }

    if (card_count < current_max_cards) { // Check if the card collection is not full
        this->card_count++; // Increment the card count 
        card->setNumber(next_card_number++); // Set the card number and increment the counter 
        card_collection[card_count] = (*card); // Add the card to the collection 
        return card->getNumber(); // Return the card number
    } else {
        log_e("Card collection is full");
        return -1; // Indicate an error
    }
}

bool CardCollection::delete_card(int card_number) { 
    // Iterate over the card collection
    for (size_t i = 0; i < card_count; ++i) {
        
        // Skip if the card number does not match
        if (card_collection[i].getNumber() != card_number) continue;
        Card* card = &card_collection[i]; // Get the card object
        
        // delete card; // Delete the card object 

        // Shift remaining cards to fill the gap
        for (size_t j = i; j < card_count - 1; ++j) {

            // Move each card after the deleted card one position back in the array
            card_collection[j] = card_collection[j + 1];  
        }
        card_count--; // Decrement the card count
        return true; // Card successfully deleted
        
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

CardCollection cardCollection;

