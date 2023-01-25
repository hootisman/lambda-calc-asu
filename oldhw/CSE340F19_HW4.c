// CSE340 Fall 2019 HOMEWORK 4
//
// DUE DATE: Wednesday November 6 2019 by 11:59 pm on canvas/gradescope
//
// This code contains a number of questions.
// You should answer the questions and provide justification for your answers.
// All questions assume that the code is compiled according to ANSI C99 
// standard. To compile it according to C99 standard execute the 
// following from the command prompt (not visual studio!)
//
//        gcc -std=gnu99 CSE340S18_HW4.c
// 

#include <stdio.h>
#include <stdlib.h>

struct T {
	int a;
	int *b;
	struct T *next;
} ;

struct T p0;
struct T *p1;
struct T **p2;
struct T **p3;

int *p;
int *q;

int main()
{
	int i;
	struct T* cursor;

	p1 = (struct T *) malloc(sizeof(struct T));
	p2 = (struct T **) malloc(sizeof(struct T *));

        p = (int *) malloc(sizeof(int));
        q = (int *) malloc(sizeof(int));

	*p = *q;

	p0.next = p1;
	(*p1).a = *p;
	(*p1).next = &p0;
	*p2 = (*p1).next;

        //------------------------------------------------------------------
        // Question1. Draw a box-circle diagram for p,q, p0, p1, p2 and the 
        //            memory allocated above at this point
        //------------------------------------------------------------------


        // scope 1
        {   struct T *a[5];
            struct T **b[5];
            int i,j;

            a[0] = (struct T*) malloc(sizeof(struct T));
            p2 = &(a[0]);
	    (*a[0]).next = p1;
        
            //-----------------------------------------------------------
            // Question 2. Draw a box-circle diagram for p1, p2, a[]
            //             and the memory allocated above at this point
            //------------------------------------------------------------
         
            for (i = 1; i < 4; i++)
            {
                a[i] = (struct T*) malloc(sizeof(struct T));
                (*a[i]).next = a[i-1];
                (*a[i]).a = i;
                b[i] = &a[i];
                (**b[i]).next = *b[i];
            }
            (*p1).next = a[2];

            //--------------------------------------------------------------
            // Question 3. Explain the output produced by the
            //             following loop
            //--------------------------------------------------------------
            cursor = a[0];
            for (j = 0; j < 5; j++)
            {
                printf("%d  ",(*cursor).a);
                cursor = (*cursor).next;
            }
            printf("\n");
            
            //--------------------------------------------------------------
            // Question 4. Draw a box-circle diagram of ALL the variables
            //             and the memory allocated up to this point?
            // 		what locations are garbage at this point?
            //--------------------------------------------------------------


            //--------------------------------------------------------------
            // Question 5. Give an alias for a[2] at this point.
            //             This alias should not include the names "a" !!
            //             This might be tricky and you might want to skip
            //             it and do other parts first.
            //--------------------------------------------------------------

            // scope 2
            {   struct T *b[6];
                int j;

		p3 = &b[2];
                for (i = 1; i < 6; i++)
                {
                   b[i] = (struct T*) malloc(sizeof(struct T));
                   (*b[i]).next = a[i%4];
                   (*b[i]).a = i;
                   (*b[i]).b = &i;
                }

                //-------------------------------------------------------
                // Question 6. Explain the output produced by the
                //             following loop 
                //-------------------------------------------------------
                for (j = 1; j < 6; j++)
                {
                     printf("%d  ",(*(*b[j]).next).a);	
                }
                printf("\n");

                //-------------------------------------------------------
                // Question 7. Explain the output produced by the
                //             following loop 
                //-------------------------------------------------------
                for (j = 1; j < 6; j++)
                {
                     printf("%d  ",(*b[j]).a);	
                }

                //-------------------------------------------------------
                // Question 8. Explain the output produced by the
                //             following printf()
                //-------------------------------------------------------
                printf("%d \n", (**p3).a);

             } // scope2 end

       
            //-----------------------------------------------------------
            // Question 9. What are the dangling references at
            //             this point?
            //-----------------------------------------------------------


            //-----------------------------------------------------------
	    // Question 10. What locations are garbage at this point?
            //-----------------------------------------------------------


	} // scope 1 end

        //----------------------------------------------------------------
        // Question 11. What are the dangling references at this point.
        //----------------------------------------------------------------
}
	
