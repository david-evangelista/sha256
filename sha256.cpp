#include "sha256.h"

/* Default Constructor. Initializes constants to be used, h1->h7, as well as the
array of round constants to be used. */
hash::hash(): h0{0x6a09e667}, h1{0xbb67ae85}, h2{0x3c6ef372}, h3{0xa54ff53a},
              h4{0x510e527f}, h5{0x9b05688c}, h6{0x1f83d9ab}, h7{0x5be0cd19},
              round_const {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
                           0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
                           0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 
                           0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 
                           0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 
                           0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 
                           0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 
                           0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 
                           0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 
                           0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 
                           0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 
                           0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 
                           0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 
                           0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 
                           0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 
                           0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2} {}
/* Parameter constructor. Calls default constructor, then also creates the
message array out of the passed string. */
hash::hash(std::string user_str_) : hash() {
    cout<<"Hashing value..."<<endl; 
    user_str = user_str_; // initializing data member
    str_to_32bit_chunks(user_str);
    append_big_end_length();
    mod_16_to_63();
    compression();
    cout<<getHash()<<endl;
}

/* Overloading stream insertion operator */
ostream& operator <<(ostream& os, hash& h){
    for (int i = 0; i < 64; i++)
        os << h.block[i] <<endl;
    return os;
}

std::string hash:: getHash(){
    return concat_final();
}

/* String to 32 bit chunks (helper function): passed string is placed into the
block array as a binary string (padded 0's at the end if the length of the string 
is not a multiple of 4) */
void hash:: str_to_32bit_chunks(std::string user_str){
    int i = 0; //index of current block
    std::string concat;    
    while (user_str.length() > 4) {
        for (int j = 0; j < 4; j++) // iterates through next 4 chars of string
            concat += (bitset<8> ( (char)user_str.at(j) ).to_string());
        block [i++] = bitset <32> (concat); // store in block and increment index
        concat = ""; // reset string
        user_str.erase(0, 4); // erase first 4 characters
    } // while

    // number of chars now left: minimum 0, maximum 3:
    for (int k = 0; k < user_str.length(); k++)//iterates through next 4 chars of str
        concat += (bitset<8> ( (char)user_str.at(k) ).to_string());

    // Appending 1 and padding 0's
    /* Assuming the string was not a multiple of 4 */
    // append a 1 and pad the rest with 0s    
    concat += (bitset<8> (128)).to_string(); // append a 1 and seven 0's
    for (int p = user_str.length(); p < 4; p++) 
        concat += (bitset<8> (0)).to_string(); // pad remaining with 0's
    // ready to store in block
    block [i++] = bitset <32> (concat); // store in block and increment index 
    /* But if the string was a multiple of 4, then no 1 was appended
    and in this case the next entry needs to be 1000... */
    if (user_str.length() == 4) // if length was 4, we need to append a 1 on a following entry
        block[i] = bitset <32> (2147483648); // then make the following block 1000...000
}

/* Considering the string size limitation to 55 chars, the length of the string
will never exceed 440 bits, and so it will always be appended in block 15 in
this implementation. */
void hash:: append_big_end_length(){
    block[15] = bitset <32> (this->user_str.length() * sizeof(char) * 8);
}

void hash:: mod_16_to_63(){
    bitset <32> s0, s1;
    for (int i = 16; i < 64; i++){
        // s0
        s0 = rotate_right_32(block[i-15], 7)
        ^ rotate_right_32(block[i-15], 18) ^ (block[i-15] >> 3);
        // s1
        s1 = rotate_right_32(block[i- 2], 17) 
        ^ rotate_right_32(block[i-2], 19) ^ (block[i-2] >> 10);   
        // block [i]
        block[i] = bitset<32> (block[i-16].to_ulong() + s0.to_ulong()
                                   + block[i-7].to_ulong() + s1.to_ulong());
    }
}
void hash:: compression(){
    bitset<32> a (h0);
    bitset<32> b (h1);    
    bitset<32> c (h2); 
    bitset<32> d (h3); 
    bitset<32> e (h4);
    bitset<32> f (h5);    
    bitset<32> g (h6); 
    bitset<32> h (h7);

    bitset <32> s0, s1, ch, temp1, maj, temp2;
    for (int i = 0; i <64; i++) {
        s1 = rotate_right_32 (e, 6) ^ rotate_right_32 (e, 11)
                                    ^ rotate_right_32 (e, 25);
        ch = (e & f) ^ (~e & g);
        temp1 = bitset<32> ( h.to_ulong() + s1.to_ulong() + ch.to_ulong()
                        + round_const[i].to_ulong() + block[i].to_ulong() );
        s0 = rotate_right_32(a, 2) ^ rotate_right_32(a, 13)
                                   ^ rotate_right_32(a, 22);
        maj = (a & b) ^ (a & c) ^ (b & c);
        temp2 = bitset<32> ( s0.to_ulong() + maj.to_ulong());
        h = g;
        g = f;
        f = e;
        e = bitset<32> (d.to_ulong() + temp1.to_ulong());
        d = c;
        c = b;
        b = a;
        a = bitset<32> (temp1.to_ulong() + temp2.to_ulong());                       
    }
    // Adding initial constants to result and storing in final_values array
    final_values[0] = bitset<32> (a.to_ulong() + h0.to_ulong());
    final_values[1] = bitset<32> (b.to_ulong() + h1.to_ulong());
    final_values[2] = bitset<32> (c.to_ulong() + h2.to_ulong());
    final_values[3] = bitset<32> (d.to_ulong() + h3.to_ulong());    
    final_values[4] = bitset<32> (e.to_ulong() + h4.to_ulong());
    final_values[5] = bitset<32> (f.to_ulong() + h5.to_ulong());
    final_values[6] = bitset<32> (g.to_ulong() + h6.to_ulong());
    final_values[7] = bitset<32> (h.to_ulong() + h7.to_ulong());
}

std::string hash:: concat_final(){
    std::stringstream hash;
    for (int i = 0; i <8; i++)
        hash << std::hex << std::uppercase << final_values[i].to_ulong();
    return hash.str();
}

bitset <32> rotate_left_32 (bitset<32> in_bitset, int n){
    bitset<32> out_bitset;
    for (int i = 0; i <32 ; i++)
            out_bitset[(i+n)%32] = in_bitset[i];
    return out_bitset;
}

bitset <32> rotate_right_32 (bitset<32> in_bitset, int n){
    bitset<32> out_bitset;
    for (int i = 0; i <32 ; i++)
            out_bitset[(32-n+i)%32] = in_bitset[i];
    return out_bitset;
}

