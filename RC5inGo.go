package main

import (

	"fmt"
)

const (
	W32 		= 32		// word size in bits
	R32         = 12 	// word size in bytes
	B32 		= 16  		// block size in bits
	C32 		= 04 
	T32 		= 26
	P32			= 0xB7E15163
  	Q32 		= 0x9E3779B9
)

var S [26]uint32

func rotl32(x uint32, y uint32) uint32 {
	return (((x) << (y & (32 - 1))) | ((x) >> (32 - (y & (32 - 1))))) 
}

func rotr32(x uint32, y uint32) uint32 {
	return (((x) >> (y & (32 - 1))) | ((x) << (32 - (y & (32 - 1)))))
}

func Encrypt(pt [2]uint32 , ct [2]uint32) ([2]uint32){

	A, B := pt[0] + S[0], pt[1] + S[1]

	for i := 1; i <= R32; i++ {
		A = rotl32(A^B, B) + S[2 * i + 0]
		B = rotl32(B^A, A) + S[2 * i + 1]
	}
ct[0] = A 
ct[1] = B 
return ct 
}

func Decrypt(ct [2]uint32, pt [2]uint32) ([2]uint32){

	A := ct[0] 
	B := ct[1]

	for i := int(R32); i > 0; i-- {
		B = rotr32(B - S[2 *i + 1], A&31) ^ A
		A = rotr32(A - S[2 *i + 0], B&31) ^ B
	}

	pt[1] = B - S[1]
	pt[0] = A - S[0]
	return pt
}

func Setup(K [16]uint32) {
	var i uint32
	var j uint32
	var k uint32
	var A uint32
	var B uint32
	var u uint32

	u = W32 / 8 
	var L [C32]uint32


	for i, L[C32 - 1] = B32 - 1, 0; i != 0; i-- {
		L[i/u] = (L[i / u] << 8) + K[i] 
	}

	for S[0] , i = P32, 1; i < T32; i++ {
		S[i] = S[i - 1] + Q32 
	}

	for A,B,i,j,k = 0,0,0,0,0; k < 3 * T32; k++ {
       
		S[i] = rotl32(S[i] + (A + B), 0000003)
        A = S[i]
        L[j] = rotl32(L[j] + (A + B), (A + B))
        B = L[j]
        i = (i + 1) % T32
        j = (j + 1) % C32

    }
	
}

func printword(A uint32){

	var k uint32
	for k = 0; k < W32 ; k+=8 {
	fmt.Printf("%2.2X", (A>>k)&0xFF)
	}
}


//main function 
func main(){

	var pt1 [2]uint32
	var pt2 [2]uint32

	ct :=[2]uint32{1000,50}
	
	 key := [16]uint32 {0x8B, 0x15, 0x2F, 0x3A,
       					0x93, 0x0D, 0x39, 0x57,
        				0xC9, 0x02, 0xDB, 0x37,
        				0x1B, 0x74, 0x90, 0x36 } 

		for i := 0; i < 1 ; i++ {
			pt1[0] = ct[0] 
			pt1[1] = ct[1]
	
				fmt.Printf("key = ")
				for j := 0; j < 16; j++ {
					fmt.Printf("%.2X ",key[j])
				}
				
				fmt.Println()
			 	Setup(key)	

					ct := Encrypt(pt1, ct)
					pt2 := Decrypt(ct, pt2)
			

				fmt.Println()
				fmt.Printf("Plaintext: ") 
				printword(pt1[0]) 
				printword(pt1[1]) //plaintext block 1 and 2 

				fmt.Printf(" ---> Ciphertext: ") 
				printword(ct[0]) 
				printword(ct[1]) // ciphertext block 1 and 2 
				
				fmt.Println()
				fmt.Printf("Decrypted plaintext: ") 
				printword(pt2[0]) 
				printword(pt2[1])

				fmt.Println()

		}


}
