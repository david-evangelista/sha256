#pragma once
#include <iostream>


/* Prompts user for input, returns a string. */
std::string userStringInput();
/* Returns true if passed string contains 63 chars or less. */
bool validateString(std::string user_str);
bool tryAgain();