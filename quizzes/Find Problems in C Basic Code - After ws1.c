/* #include <stdio.c>  ---------------------- Missing .h instead of the .c */ 
#include <stdio.h>
/*
void Main(int argc, char *argv[])
{       
    int num;
    const int num2 = 6;         
    const int num3;         

    printf("%d", num);
    scanf("%d", &num); /* expects arguments of type int* (scanf)) 
    printf("100 divided by %d is %f", num, 100/num); /* (100/num) is int - and here we said that is %f expects argument of type '%d' - int        

    char ch = 97; /* ISO C90 forbids mixed declartions and code , we to declare at the top of the scope . 
    
    num3 = num2 – 5; /* we cannot assign to an const int type .
    printf("%f", num3);      
    
    num = 4.7;      
    (double)num = 4.7;
            
    num = 2 * num++;                
    
    return (0);     
}  

*/


int main(int argc, char *argv[])
{       
    int num;
    const int num2 = 6;         
    const int num3;    
    char ch;     

    printf("%d", num);
    printf("100 divided by %d is %d", num, 100/num); /* (100/num) is int - and here we said that is %f expects argument of type '%d' - int */          

    ch = 97; 
    
    printf("%f", num3); /* need to be %d */
    
    num = 4.7;  /* make implict cast to int (4.7 become 4)*/    
    num = 4.7; /* lvalue required as left operand of assigment (double)num is a temp var and he dont have an address to store the value */
            
    num = 2 * num++;          
    
    return (0);     
}  