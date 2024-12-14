#ifndef CARD_HPP
#define CARD_HPP

#include "includes.hpp"

class Card {
public:
    Card(const char* uid, int number, const char* name, float balance);

    char* getUID() const;
    int getNumber() const;
    char* getName() const;
    float getBalance() const;
    void setName(const char* name);
    void setBalance(float balance);

private:
    char* uid;
    int number;
    char* name;
    float balance;
};

class CardCollection {
public:
    CardCollection(int initial_size = 6);
    ~CardCollection();

    int save_card(Card* card);
    bool delete_card(int card_number);
    Card* find_card_by_number(int card_number);
    Card* find_card_by_uid(const char* uid);
    bool update_card_collection_size(int size_change);

private:
    Card* card_collection;
    size_t card_count;
    int next_card_number;
    int current_max_cards;
};

#endif // CARD_HPP