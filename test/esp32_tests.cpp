#include <Arduino.h>
#include <unity.h>
#include "tests.hpp" // include the header file with the test functions

void setup() {
    delay(2000); // service delay
    UNITY_BEGIN();

    // set stuff up here 

    numCard1 = cards.save_card(card1);
    numCard2 = cards.save_card(card2);

    RUN_TEST(test_card12_not_equal);
    RUN_TEST(test_card1_uid);
    RUN_TEST(test_card1_number);
    RUN_TEST(test_card1_name);
    RUN_TEST(test_card1_balance);
    RUN_TEST(test_card2_uid);
    RUN_TEST(test_card2_number);
    RUN_TEST(test_card2_name);
    RUN_TEST(test_card2_balance);
    RUN_TEST(test_add_card3);
    RUN_TEST(test_delete_card4);
    RUN_TEST(test_card_collection_overflow);

    cards.delete_card(numCard1);
    cards.delete_card(numCard2);

    // delete card1;
    // delete card2;


    RUN_TEST(test_format_uid_to_hex_string_empty_uid);
    RUN_TEST(test_format_uid_to_hex_string_valid_uid);
    RUN_TEST(test_format_uid_to_hex_string_small_buffer);

    UNITY_END();

    setupMain();
}

void loop() {
    loopMain();
    // run tests
}