#pragma once
#ifndef CARD_HPP
#define CARD_HPP

#include <includes/includes.hpp>
 
class Card {
public:
    Card(const char* uid, const char* name, int32_t balance);
    Card(const Card& card); 
    Card() {};
    ~Card();
    bool operator==(const Card* card);
    bool operator==(const Card& card);
    Card& operator=(const Card& other);
    char* getUID() const;
    int getNumber() const;
    char* getName() const;
    int32_t getBalance() const;
    void setNumber(int number);
    void setUID(const char* uid);
    void setName(const char* name);
    void setBalance(int32_t balance);

private:
    char* uid = nullptr;
    int number = 0;
    char* name = nullptr;
    int32_t balance = 0;
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
    int getNextCardNumber() { return next_card_number; }
private:
    Card* card_collection;
    uint8_t card_count = 0 ;
    int next_card_number;
    int current_max_cards;
};

extern CardCollection cardCollection;

#endif // CARD_HPP