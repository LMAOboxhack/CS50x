#include <cs50.h>
#include <stdio.h>

void space(int i);
void matter(int y);

int main(void)
{           
    int height ;
    do
    {
        height = get_int("Height: ");
    } 
    while (height<1 || height>8);

    for (int n=1; n<= height; n++)
    {
        int blank = abs(n-height);
        space(blank);
        matter(n);
        printf("  ");
        matter(n);
        printf("\n");
    }  
}


//Function for space
void space(int i)
{
    for (int p=0; p<i; p++)
    {
        printf(" ");
    }
}


//Function for matter
void matter(int y)
{
    for (int q=0; q<y; q++)
    {
        printf("#");
    }
}
