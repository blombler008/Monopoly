#include "tests.hpp" // include the header file with the test functions


void test_format_uid_to_hex_string_empty_uid() {
    char tag[20];
    MFRC522::Uid uid = {0, {0}, 0};
    helper::format_uid_to_hex_string(tag, &uid, sizeof(tag));
    TEST_ASSERT_EQUAL_STRING("0x0", tag);
}

void test_format_uid_to_hex_string_valid_uid() {
    char tag[20];
    MFRC522::Uid uid = {4, {0xDE, 0xAD, 0xBE, 0xEF}, 4};
    helper::format_uid_to_hex_string(tag, &uid, sizeof(tag));
    TEST_ASSERT_EQUAL_STRING("0xde-0xad-0xbe-0xef", tag);
}

void test_format_uid_to_hex_string_small_buffer() {
    char tag[10];
    MFRC522::Uid uid = {4, {0xDE, 0xAD, 0xBE, 0xEF}, 4};
    helper::format_uid_to_hex_string(tag, &uid, sizeof(tag));
    TEST_ASSERT_EQUAL_STRING("", tag); // Expecting an empty string due to buffer too small
}
