#include <cs50.h>
#include <string.h>
#include <stdio.h>
#include <crypt.h>

string gen(int i, string arg, string salt);

//length of guess
int pwlen = 3;
char pw[6];

//set of characters ie upper & lower case alphabet
string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
int setlen = 52;

//counter for number of combinations - to check
int count = 0;

int main(int argc, string argv[])
{
    string hash = argv[1]; 
    
    //obtains salt from command line input 
    char salt[3];
    for (int i=0; i < 2; i++)
    {
        salt[i] = hash[i];
    }

    //generation function
    //
    for (int a = 0; a < pwlen; a++)
    {        
        gen(a, hash, salt);
    }

}

//self calling generator function
string gen(int i, string hash, string salt)
{    
    for (int d = 0; d < setlen; d++)
    {
        if (i < pwlen)
        {
            pw[i] = charset[d];
            
            //test
            string hashguess = crypt(pw, salt);
            string lol = "Azz";
            if (strncmp (hashguess, hash, 13) == 0)
               //(hashguess == hash)
            {
                printf("%s\n", pw);
                exit(0);
            }
            else
            {  
                gen(i+1, hash, salt);
            }
        }
        else
        {
        }
    }
    return 0;
}


