#include <stdio.h>
#include <cs50.h>

int getdigit(int i);
long number(long l);

int main(void)
{ 
    long number = get_long("Number: ");   
    
    
    //Counter for number's length
    int counter = 0;
    for(long i = number; i > 10; i /= 10)
    {
        counter++;
    }
    counter++;
    int length = counter;
        

    //Luhn's algorithm (checksum)
        //Part 1
    int chksum = 0;
    for (long x = number; x > 9; x /= 100)
    {
        int evendig = ((x%100)/10)*2;
        if (evendig > 9)
        {
            chksum += (evendig/10) + (evendig%10);
        }
        else
        {
            chksum += evendig;
        }
    }
        //Part 2
    for (long x = number; x > 0; x /= 100)
    {
        int odddig = x%10;
        chksum += odddig;
    }
    //printf("Checksum = %i\n", chksum);
    
    
    //Validity of checksum -- valid if 1; invalid if 0
    int valid = 0;
    if (chksum%10 == 0 && (length == 13 || length == 15 || length == 16) )
    {
        valid += 1;
    }
    else
    {
        //valid = 0
    }
    
    //printf("Valid = %i\n", valid);    
   
    //Getting the first digits -- 34/37 for AMEX; 51-55 for MASTER; 4 for VISA
    long first = number;
    while (first > 99)
    {
        first /= 10;
    }
    //printf("First digit(s) = %ld\n", first);   
    
    //Checking for card type using if/else
    if (valid ==1)
    {
        if (first/10 == 4)
        {
            printf("VISA\n");
        }
        else if (first == 51 || first == 52 || first == 53 || first == 54 || first == 55)
        {
            printf("MASTERCARD\n");
        }
        else if (first == 34 || first == 37)
        {
            printf("AMEX\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else //if valid = 0
    {
        printf("INVALID\n");
    }
    
}


