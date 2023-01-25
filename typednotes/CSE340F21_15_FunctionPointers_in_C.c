// Copyright Rida Bazzi 2019
#include <stdio.h>



int F(int);    // F is a function that takes an int parameter and
	           // returns an int value. This is just a declaration.


int F(int x)   // now we are defining F by providing a body for it
{
	return 4*x;
}

int times2(int x)
{
   return 2*x;
}

int times3(int x)
{

    return 3*x;
}


// what is f ?
// before attempting to understand this declaration,
// read the examples below first
int  (*f(int (*y[])(int), int x))(int)
{
    return y[x];
}


int main()
{
    int a[10];
    
    if (a == &a)
        printf("a == &a\n");
    else
        printf("a != &a\n");

    if (a == *a)
        printf("a == *a\n");
    else
        printf("a != *a\n");

     // In our box-circle diagram notation
     //              ______
     //       F ----|      |
     //             |      |
     //             |______|
     //
     // so the memory associated with F has the code for 
     // F. &F is the address of the memory associated with 
     // F which is the same as the value for F in C 
        if (F == &F)
           printf("F is equal to &F\n");
        else
           printf("F is not equal to &F");

     // also F is equal to *F. The reason for this
     // is not as clear. I believe it is notation convenience.
        if (F == *F)
           printf("F is equal to *F\n");
        else
           printf("F is not equal to *F");
 
     // note that we cannot say F = times2 
     // the fact that F == &F == *F is only for 
     // functions that are not explicitly declared
     // as pointers. For functions that are declared
     // as pointers, it is not the same. See below.

     // *y is a function that takes an int and returns 
     // an int. y is a pointer to a function that takes
     // an int and returns an int. We call y as follows
     // i = (*y)(3). Notice how the call has the same 
     // form as the declaration.

     int (*y)(int);

     // the declaration has the form int Y   where Y is (*y)(int)
     // so (*y)(int) is an int
     // (*y) is a function from int to int
     // y is a pointer to a function from int to int

     // h0 is a function that takes an int parameter and 
     // returns an int
     // assign h0 to y and the invoke y
     y = times2;
     printf("y = times2. (*y)(3) = %d \n", (*y)(3)); 
     printf("y = times2.  y(3)   = %d \n", y(3)); 

     y = times3;
     printf("y = times3. (*y)(3) = %d \n", (*y)(3)); 
     printf("y = times3.  y(3)   = %d \n", y(3)); 
     //  y is not the same as &y 
        if (y == &y)
           printf("y is equal to &y\n");
        else
           printf("y is not equal to &y\n");

     // but y is the the same as *y
        if (y == *y)
           printf("y is equal to *y\n");
        else
           printf("y is not equal to *y\n");

     // -----------------------------------------------------
     // k is an array of function pointers. See explanation below
     int (*k[3])(int);

     // -----------------------------------------------------
     // EXPLANATION for int (*k[3])(int);
     // -----------------------------------------------------
     // this is of the form
     //
     //    int K;
     //
     // where K is (*k[3])(int)
     //
     // so     (*k[3])(int) is an int
     //
     // (*k[3])(int) is  (*k[3]) applied to an int value
     // so, when we apply (*k[3]) to an int value we get an int
     // so   (*k[3]) must be a function from int to int
     // 
     // if *k[3] is a function from int to int, then
     // k[3] is a pointer to a function from int to int
     // 
     // It follows that k is an array of pointers to functions
     // from int to int
     // -----------------------------------------------------

     // -----------------------------------------------------
     // EXPLANATION for int  (*f(int (*y[])(int), int x))(int)
     // -----------------------------------------------------
     // now we can explain the more complicated declaration of f
     //
     // The declaration has the form
     //
     //        int (...)(int)
     //
     // where (...) is (*f(int (*y[])(int), int x))
     //
     // so (...) is a function from int to int
     //
     // (*f(int (*y[])(int), int x)) is a function from int to int
     //
     // it has the form
     //
     //     *f ( Y1, X1).
     //
     // where
     //
     //    1. Y1 = int (*y[])(int)
     //    2. X1 is int x
     //
     // *f(Y1,X1) is a function from int to int
     //
     // *f takes two parameters and returns a function from int to int
     //
     // 1. Y1 is similar to the k declaration we have just seen
     //
     //    (*y[]) is a function from int to int
     //    y[] is a pointer to a function from int to int
     //    y is an array of pointers to function from int to int
     //
     //    Y1 is an array of pointers to functions from int to int
     //
     // 2. X1 is int
     //
     // *f is a function that takes an array of function pointers and an integer
     // and returns a function from int to int
     //
     // f is a pointer to a function that takes an array of function pointers
     // and an integer and returns a function from int to int

     k[0] = times2;
     k[1] = times3;
     printf("k[0](3) =  %d \n", k[0](3));
     printf("k[1](3) = %d \n", k[1](3));

     printf("((*f)(k,0))(3) = %d \n", ((*f)(k,0))(3));
     printf("((*f)(k,1))(3) = %d \n", ((*f)(k,1))(3));
       
}

