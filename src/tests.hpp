#ifndef TEST_TESTS_HPP
#define TEST_TESTS_HPP
#include <Arduino.h>
#include <unity.h>
#include "main.hpp"

extern Card* card1;
extern Card* card2;
extern CardCollection cards;
extern int numCard1;
extern int numCard2;

void test_card12_not_equal();
void test_card1_uid();
void test_card1_number();
void test_card1_name();
void test_card1_balance();
void test_card2_uid();
void test_card2_number();
void test_card2_name();
void test_card2_balance();
void test_add_card3();
void test_delete_card4();
void test_card_collection_overflow();

void test_format_uid_to_hex_string_empty_uid();
void test_format_uid_to_hex_string_valid_uid();
void test_format_uid_to_hex_string_small_buffer();
 

#endif // TEST_TESTS_HPP