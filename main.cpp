#include "sha256.h"
#include "io.h"

int main()
{
    do {
    std::string user_str = userStringInput();  
    hash h(user_str);
    } while (tryAgain());
    cout<< "\nGoodbye!"<<endl;
    return 0;
}

