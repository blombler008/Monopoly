#include <Arduino.h>
#include <unity.h> 
#include "tests.hpp" // include the header file with the test functions
#include "card.hpp" // include the header file with the class definition
 
Card* card1 = new Card("1234567890", "TestCard1", 100.0f);
Card* card2 = new Card("ABCDEFGHIJK", "TestCard2", 200.0f);
CardCollection cards(6);
int numCard1 = 0;
int numCard2 = 1;

void test_card12_not_equal() {
    TEST_ASSERT_FALSE(*card1 == *card2);
}

void test_card1_uid() {
    TEST_ASSERT_EQUAL_STRING("1234567890", card1->getUID());
}

void test_card1_number() {
    TEST_ASSERT_EQUAL(numCard1, card1->getNumber());
}

void test_card1_name() {
    TEST_ASSERT_EQUAL_STRING("TestCard1", card1->getName());
}

void test_card1_balance() {
    TEST_ASSERT_EQUAL_FLOAT(100.0f, card1->getBalance());
}

void test_card2_uid() {
    TEST_ASSERT_EQUAL_STRING("ABCDEFGHIJK", card2->getUID());
}

void test_card2_number() {
    TEST_ASSERT_EQUAL(numCard2, card2->getNumber());
}

void test_card2_name() {
    TEST_ASSERT_EQUAL_STRING("TestCard2", card2->getName());
}

void test_card2_balance() {
    TEST_ASSERT_EQUAL_FLOAT(200.0f, card2->getBalance());
}

void test_add_card3() {
    Card* new_card = new Card("NEWCARDUID", "NewCard", 300.0f);
    const int number = cards.save_card(new_card);
    Card found_card = *cards.find_card_by_uid("NEWCARDUID");

    const char* uid = found_card.getUID();
    const char* name = found_card.getName(); 
    const float balance = found_card.getBalance();

    TEST_ASSERT_NOT_NULL(&found_card);
    TEST_ASSERT_EQUAL_STRING(new_card->getUID(), uid);
    TEST_ASSERT_NOT_EQUAL(0, number);
    TEST_ASSERT_EQUAL_STRING(new_card->getName(), name);
    TEST_ASSERT_EQUAL_FLOAT(new_card->getBalance(), balance);
    delete new_card;
}

void test_delete_card4() {
    Card* new_card = new Card("DELETEUID", "DeleteCard", 400.0f);
    const int number = cards.save_card(new_card);
    bool delete_result = cards.delete_card(number);
    Card* found_card = cards.find_card_by_number(number);

    TEST_ASSERT_TRUE(delete_result);
    TEST_ASSERT_NULL(found_card);
    delete new_card;
}

void test_card_collection_overflow() {
    Card* overflow_card = nullptr;
    int result = 0;
    int i = 0;
    char uid[12];
    // Create cards until save_card returns -1
    while (result != -1) { 
        snprintf(uid, sizeof(uid), "OVERFLOW%02d", i); // Generate unique UID
        overflow_card = new Card(uid, "OverflowCard", 500.0f + i);
        result = cards.save_card(overflow_card);
        if (result != -1) {
            delete overflow_card;
        }
        i++;
    }

    // Check that the last card is not in the collection
    Card* found_card = cards.find_card_by_uid(uid);
    TEST_ASSERT_NULL(found_card);
    delete overflow_card;
}