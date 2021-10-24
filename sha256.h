#pragma once
#include <iostream>
#include <sstream>
using std::cout;
using std::cin;
using std::endl;
using std::ostream;
#include <bitset>
using std::bitset;

class hash{
    public:
    hash ();
    hash (std::string user_str_);
    friend ostream& operator<< (ostream& os, hash& h);
    std::string getHash();

    private:
    void str_to_32bit_chunks(std::string user_str); // str to bin & places in block[]
    void append_big_end_length(); // appends length of string
    void mod_16_to_63(); // prepares values in block array
    void compression(); // sets values in final_values array
    std::string concat_final(); // returns a string of 256 bits of final hash

    /* User input */
    std::string user_str;
    /* Initial constants */
    const std::bitset <32> h0;
    const std::bitset <32> h1;
    const std::bitset <32> h2;
    const std::bitset <32> h3;
    const std::bitset <32> h4;
    const std::bitset <32> h5;
    const std::bitset <32> h6;
    const std::bitset <32> h7;    

    /* Array of Round constants */
    const bitset <32> round_const [64]; // array of 32-bit entries    
    /* Array of Blocks */
    bitset <32> block [64]; // array of 32-bit entries
    /* Array of Final Hash values (8x32-bit values, which concatenated give 256 bits) */
    bitset <32> final_values [8];

};


bitset <32> rotate_left_32 (bitset<32> inb, int n);
bitset <32> rotate_right_32 (bitset<32> inb, int n);
