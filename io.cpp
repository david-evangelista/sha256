#include "io.h"
using std::cout;
using std::cin;
using std::endl;

std::string userStringInput(){
    cout << "\nPlease enter a string of 55 characters or less." << endl;
    std::string user_str;
    std::getline( std::cin, user_str);
    cout<<endl; // format
    while (!validateString(user_str)) {
        cout << "The string must be 55 characters or less. Please try again."<<endl;
        std::getline( std::cin, user_str);
    }
    cout<<endl; // format
    return user_str;
}

bool validateString(std::string user_str){
    return (user_str.length() < 64);
}

bool tryAgain(){
    cout<< "\nWould you like to try again? Enter Y for yes. Press any other key sequence to exit."<<endl;
    std::string y_n;
    cin >> y_n;
    cin.ignore(); // discards rest of input
    if ((y_n) == "y" ||  (y_n) == "Y")
        return true;
    else return false; 
}

