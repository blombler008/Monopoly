#pragma once
#ifndef CARD_HPP
#define CARD_HPP

#include "includes.hpp"

 
class Card {
public:
    Card(const char* uid, const char* name, float balance);
    Card(const Card* card);
    Card() {};
    ~Card();
    bool operator==(const Card* card);
    bool operator==(const Card& card);
    char* getUID() const;
    int getNumber() const;
    char* getName() const;
    float getBalance() const;
    void setNumber(int number);
    void setUID(const char* uid);
    void setName(const char* name);
    void setBalance(float balance);

private:
    char* uid = new char[0];
    int number = 0;
    char* name = new char[0];
    float balance = 0;
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
    bool is_uid_unique(const char* uid);
private:
    Card* card_collection;
    size_t card_count;
    int next_card_number;
    int current_max_cards;
};

extern CardCollection cardCollection;
  

#endif // CARD_HPP