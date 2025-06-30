// BufferOverflow.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <cstring>

int main()
{
    std::cout << "Buffer Overflow Example" << std::endl;

    // TODO: The user can type more than 20 characters and overflow the buffer, resulting in account_number being replaced -
    //  even though it is a constant and the compiler buffer overflow checks are on.
    //  You need to modify this method to prevent buffer overflow without changing the account_number
    //  variable, and its position in the declaration. It must always be directly before the variable used for input.
    //  You must notify the user if they entered too much data.

    const std::string account_number = "CharlieBrown42";
    std::string temporary_buffer; // Temporary buffer to hold user input
    char user_input[20];
    while (true)
    {
        // Prompt the user for the value
        std::cout << "Enter a value (max 20 characters): ";

        // read user input into the temporary buffer
        std::getline(std::cin, temporary_buffer);

        // Check if the input exceeds the buffer size
        if (temporary_buffer.length() > sizeof(user_input))
        {
            std::cout << "Input too long! Please enter a maximum of 20 characters." << std::endl;
            continue; // Prompt again
        }
        else
        {
            // Copy the input to the user_input buffer
            std::strncpy(user_input, temporary_buffer.c_str(), sizeof(user_input));
            user_input[sizeof(user_input) - 1] = '\0'; // Null-terminate the string
            break; // Exit the loop if input is valid
        }
    }

    // Output the account number and user input
    std::cout << std::endl;
    std::cout << "You entered: " << user_input << std::endl;
    std::cout << "Account Number = " << account_number << std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
