

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

typedef uint32_t WORD;              // Should be 32-bit = 4 bytes

#define w 32                            // word size in bits
#define r 12                            // number of rounds
#define b 16                            // number of bytes in key
#define c 04                            // number words in key = ceil(8*b/w)
#define t 26                            // size of table S = 2*(r+l) words

WORD S[t];                              // expanded key table
WORD P = 0xB7E15163, Q = 0x9E3779B9;    // magic constants

// Rotation operators, x must be unsigned, to get logical right shift
#define ROTL(x, y) (((x) << (y & (w - 1))) | ((x) >> (w - (y & (w - 1))))) // Rotate Left
#define ROTR(x, y) (((x) >> (y & (w - 1))) | ((x) << (w - (y & (w - 1))))) // Rotate Right


// 2 WORD input pt (plain text) / output ct (encrypted text)
void RC5_Encrypt(WORD *pt, WORD *ct) {
    WORD i,
    A = pt[0] + S[0],
    B = pt[1] + S[1];
    for (i = 1; i <= r; i++) {
        A = ROTL(A ^ B, B) + S[2 * i + 0];
        B = ROTL(B ^ A, A) + S[2 * i + 1];
    }
    ct[0] = A, ct[1] = B;
}

// 2 WORD input ct (encrypted text) / output pt (plain text)
void RC5_Decrypt(WORD *ct, WORD *pt){
    WORD i,
    A = ct[0],
    B = ct[1];
    for (i = r; i > 0; i--) {
        B = ROTR(B - S[2 * i + 1], A) ^ A;
        A = ROTR(A - S[2 * i + 0], B) ^ B;
    }
    pt[0] = A - S[0], pt[1] = B - S[1];
}

// secret input key K 
void RC5_Setup(uint32_t *K) {
    WORD i, j, k, A, B,
    u = w / 8, L[c]; 
    // Initialize L, then S, then mix key into S
    for (i = b - 1, L[c - 1] = 0; i != 0; i--) {
        L[i / u] = (L[i / u] << 8) + K[i];
             //   printf("%d", L[i/u]);
        	//	printf(" L is:  %2.2X   ", (L[i / u] << 8) + K[i]);

              //  printf("   K:|  %d  |", K[i]);

        //      printf("   I is   %d   ", i);
    }

    for (S[0] = P, i=1; i < t; i++) 
    {   
        S[i] = S[i - 1] + Q;
      //  printf("  S is    %2.2X", S[i]);
    }

    // 3 * t > 3 * c 
    for (A = B = i = j = k = 0; k < 3 * t; k++) {
        A = S[i] = ROTL(S[i] + (A + B), 0000003);
        B = L[j] = ROTL(L[j] + (A + B), (A + B)); 
        i = (i + 1) % t;
        j = (j + 1) % c;
    //    printf("  A is  %2.2X",A);
  //  printf("  B is  %2.2X",B);
   // printf("k : %d",k);
    //printf("\n\n");
    }
    
}
// prints out the pt and ct for 8 bits by setting 2 char long and precision max of 2 char
// X gives hex conversion in uppercase A to F 
void printword(WORD A)
{ WORD k;
for (k=0;k<w;k+=8) printf("%2.2X",(A>>k)&0xFF);
}

// main function 
void main(){ 
    WORD i, j, k, pt1[2], pt2[2], ct[2] = {1000,50}; //making the first plaintext for 1st and 2nd block 
//key values
uint32_t key[b] =  { 
        0x8B, 0x15, 0x2F, 0x3A,
        0x93, 0x0D, 0x39, 0x57,
        0xC9, 0x02, 0xDB, 0x37,
        0x1B, 0x74, 0x90, 0x36 };
       /* 
    if (sizeof(WORD)!=4)
    {
        printf("RC5 error: WORD has %d bytes.\n",sizeof(WORD));
    }
*/
for (i=1;i<3;i++)
{       // Initialize pt1 and key pseudorandomly based on previous ct 
    
    pt1[0]=ct[0]; 
    pt1[1]=ct[1];

 //   for (j=0;j<b;j++) 
//    key[j] = ct[0]%(255-j);    
        
// Setup, encrypt, and decrypt 
        RC5_Setup(key);
        RC5_Encrypt(pt1,ct);
        RC5_Decrypt(ct,pt2);
// Print out results, checking for decryption failure 
    printf("key = ");
      //  for (j=0; j<b; j++) printf("%02.2X",key[j]);
      for (j = 0; j < b; j++) printf("%.2X ", key[j]); // As Hex 
      
    printf("\nPlaintext: "); printword(pt1[0]); printword(pt1[1]); //plaintext block 1 and 2 
    printf(" ---> Ciphertext: "); printword(ct[0]); printword(ct[1]); // ciphertext block 1 and 2 
    printf("\nDecrypted plaintext: "); printword(pt2[0]); printword(pt2[1]); //decrypted plaintext block 1 and 2 

        printf("\n\n");


if (pt1[0]!=pt2[0] || pt1[1]!=pt2[1]) //check if plaintext dont match 
printf("Decryption Error!");
}
}
