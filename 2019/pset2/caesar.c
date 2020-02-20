#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void checkdec(string s);
int VALID;

//check if there are 2 arguments
int main(int argc, string argv[])
{
    string KEY = argv[1];
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
        // if no. of argument != 2, program terminates with return code = 1
    }
    else
    {
        checkdec(KEY); //check validity of key
    }
    
    //encrypt plaintext if key is valid 
    if (VALID == 0)
    {
        //prompt for plaintext
        string plaintext = get_string("plaintext: ");

        //conversion to ciphertext
        int txtlen = strlen(plaintext);
        string ciphertext = plaintext;
        int key = atoi(KEY);
        for (int i = 0; i < txtlen; i++)
        {
            char Char = ciphertext[i];
            if (Char > 64 && Char < 91)
            {
                ciphertext[i] = 64 + (((ciphertext[i] - 64) + key) % 26);
            }
            else if (Char > 96 && Char < 123)
            {
                ciphertext[i] = 96 + (((ciphertext[i] - 96) + key) % 26);
            }
            else
            {
            }
        }
        printf("ciphertext: %s\n", ciphertext);
        return 0;
        }
    else
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
}

//function to check for decimal digit - if VALID != 0, KEY is invalid.
void checkdec(string s)
{
    int length = strlen(s);
    for (int n = 0; n < length; n++)
    {       
        if (isdigit(s[n]) == 0)
        {
            VALID++;
            break;
        }
        else
        {
        }
    }
}
