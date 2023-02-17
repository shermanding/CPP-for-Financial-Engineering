#include <iostream>
#include <cstring>
#include <cctype>

char row1[] = {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p'};
char row2[] = {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l'};
char row3[] = {'z', 'x', 'c', 'v', 'b', 'n', 'm'};

// declare char array size of rows + null character
char output1[11] = {};
char output2[10] = {};
char output3[8] = {};

int main(int argc, char *argv[])
{
    // Checking command=line argument
    if (argc == 1)
    {
        std::cout << "Usage: ./ex1 INPUT" << std::endl;
        return 1;
    }

    int row1Size = sizeof(row1)/sizeof(row1[0]);
    int row2Size = sizeof(row2)/sizeof(row2[0]);
    int row3Size = sizeof(row3)/sizeof(row3[0]);

    // iterate through each char in char array of argvector
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        char l1 = 0;
        char l2 = 0;
        char l3 = 0;

        // row1 of keyboard
        for (int j = 0; j < row1Size; j++)
        {
            if (i == 0)
            {
                if (tolower(argv[1][i]) == row1[j])
                {
                    l1 = 'o';
                    strcat(output1, &l1);
                }
                else
                {
                    l1 = '.';
                    strcat(output1, &l1);
                }
            }
            else
            {
                if (tolower(argv[1][i]) == row1[j])
                {
                    output1[j] = 'o';
                }
            }
        }
        // row2 of keyboard
        for (int k = 0; k < row2Size; k++)
        {
            if (i == 0)
            {
                if (tolower(argv[1][i]) == row2[k])
                {
                    l2 = 'o';
                    strcat(output2, &l2);
                }
                else
                {
                    l2 = '.';
                    strcat(output2, &l2);
                }
            }
            else
            {
                if (tolower(argv[1][i]) == row2[k])
                {
                    output2[k] = 'o';
                }
            }
        }
        // row3 of keyboard
        for (int m = 0; m < row3Size; m++)
        {
            if (i == 0)
            {
                if (tolower(argv[1][i]) == row3[m])
                {
                    l3 = 'o';
                    strcat(output3, &l3);
                }
                else
                {
                    l3 = '.';
                    strcat(output3, &l3);
                }
            }
            else
            {
                if (tolower(argv[1][i]) == row3[m])
                {
                    output3[m] = 'o';
                }
            }
        }
    }
    // output for row1
    for (int i = 0; i < strlen(output1); i++)
    {
        std::cout << output1[i] << " ";
    }
    std::cout<<std::endl;

    // output for row2
    for (int i = 0; i < strlen(output2); i++)
    {
        std::cout << " " << output2[i];
    }
    std::cout<<std::endl;

    // output for row3
    for (int i = 0; i < strlen(output3); i++)
    {
        if (i == 0)
        {
            std::cout << "  " << output3[i];
        }
        else
        {   
            std::cout << " " << output3[i];
        }
    }
    std::cout << std::endl;
}