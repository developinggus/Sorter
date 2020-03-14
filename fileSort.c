//ass0 214//
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<ctype.h>
#include<string.h>
#include<errno.h>
#include<stdint.h>
//-----------------------------------------------------------------------------
typedef struct node{
    char* str;			//linked list to store words/numbers
    struct node* next;		//made into doubly LL
    struct node* prev;
}node;
//-----------------------------------------------------------------------------
typedef struct DD{
    char** arr; //2D array
    int size;   //size of it (length)
}DD;
//-----------------------------------------------------------------------------
int areEqual(char* s1, char* s2);
int isItNumOrLet(char x);
node* insertLL(node* head, char* temp);
void printList(node* head);
//int insertionSort(void* toSort, int (*comparator)(void*,void*));
int insertionSort(void* toSort, int (*comparator) (void*, void*));
int quickSort(void* toSort, int (*comparator)(void*,void*));
int intcmp(void* int1, void* int2);
int stringcmp(void* str1, void* str2);
int realQuickSorter( char **A, int start, int end, int(*comparator)(void*,void*));
void recQuick(char **A, int start, int end, int(*comparator)(void*,void*));
node* bringMeEnd( node* temp);
void swap (char **s1, char **s2);
//void sortedInsert( node* head, node* nodeToInsert, int (*comparator)(void*, void*));
void swapper(char ***a, char ***b);
int countMe(node *a);
void printArr(char** A, int size, char ip);
int matchCheck(node* a, char ip);
void nodeDestroy(node* head);
void ddDestroy(DD* Box);
//-----------------------------------------------------------------------------
//What do ignore!
//Tabs/Newlines/Spaces/
//
//
//Reading in:
//Have a temp string of size __?
//
//Read in character my character, (buffer size will be one byte)
//
//Have a int called p, it will point to a certain element in the string, starting for 0-size_of_string
//
//If the char read in is a num or letter (methods  will be created for this) we will insert it into the dummy string, if not a letter a number or
//comma, we will ignore it, if it is a comma, we send the string to a method that will create the linked list, then we wipe the temp string and start again
//
//When status finally reads less than 0, it means we are done reading the file
//
//We will then check to see if it is '-q' or '-i' to ship to the app. sorter
//
//--Insertion Sort---
//
//
//--Quicksort---
//1st element is a pivot
//Create two linked lists, one those is shorter than the pivot and one that is larger
//shorter-pivot-longer
//-----------------------------------------------------------------------------
int main (int argc, char** argv) {

    /* argv[0] = "filesort"
    *  argv[1] = -1 or -q
    *  argv[2] = somefile
    */

    char *blank = "";

    int fd = open(argv[2], O_RDONLY);

    int status = 1;

    char buffer[20];

    buffer[19] = '\0';

    int p,q;

    char * str;

    str = argv[2];

    if ( fd < 0 ) {
        //str = strerror(errno);
        printf("Fatal Error: '%s' Does Not Exist\n", str);
        exit(1);
    }

    if ( argc > 3 ) {
        //str = strerror(errno);
        printf("Fatal Error: Too Many Arguments\n");
        exit(1);
    }

    if ( argc > 0 && argc < 3 ) {
        //str = strerror(errno);
        printf("Fatal Error: Too Little Arguments\n");
        exit(1);
    }

    p = 0;
    char ip;
    char * temp = (char *) malloc(50*sizeof(char)); //temp string to hold the string before we turn it into a node
    memset(temp,'\0',50);
    //printf("temp after first memset is %d\n",strlen(temp));

    node* head = NULL;
    int ln = 50;
    int readIN = 0;
    while ( status > 0 ) {
        //readIN++;
        status = read(fd, buffer, 1);

        q = isItNumOrLet(buffer[0]);

        if ( q == 1 ) { //its indeed a number of letter

            if ( readIN == 0 ) {
                ip = buffer[0]; //this is just to be ableto check later on if were dealing with nums or letters
                readIN++;
            }

            if ( p >= ln ) {
                //printf("I FIREDDDDD\n");
                char * temp2 = (char *) malloc((ln * 2) * sizeof(char));
                memset(temp2,'\0',ln *2);
                strcpy(temp2,temp);
                temp = temp2;
                ln = ln * 2;
                //temp = (char *) realloc(temp,strlen(temp) * 2);
            }

            temp[p] = buffer[0];
            p++;
        }

        if ( buffer[0] == ',' ) {

            head = insertLL(head,temp);
            char * temp3 = (char *) malloc((50 * sizeof(char)));
            memset(temp3,'\0',50);
            temp = temp3;
            p = 0;
            ln = 50;
        }
    }

    if (temp[0] != '\0') { //the last string is in it

        head = insertLL(head,temp);
    }

    if ( head == NULL ) {
        printf("Warning: Empty text file\n");
        //return 0; //blank text file
    }

    if ( (matchCheck(head,ip)) == 0 ) {
        printf("Fatal Error: Different Types\n");
        exit(0);
    }
    //printf("%s\n",temp);

    int (*comparator)(void*,void*);

    //printf("&\n");
    //printList(head);
    //printf("&\n");
    int size = countMe(head);
    //printf("SIZE IS %d\n",size);

    char** mock = (char**) malloc(size * sizeof(char*));

    int i,m,row;

    node* ptr = head;

    row = 0;

    //printList(head);

    for ( i = 0; i < size; i++ ) {

        m = strlen(ptr->str);

        if ( row < m ) {

            row = m;

        }

        mock[i] = (char*) malloc(m * sizeof(char));         //moving contents of Linked list to 2D array

        swap(&(mock[i]),&(ptr->str));

        ptr = ptr->next;
    }

    DD* Box = (DD*) malloc(sizeof(DD));

    Box->arr = (char**) malloc( (size*row) * sizeof(char*));

    swapper(&(Box->arr),&(mock));

    Box->size = size;
    int ho = 0;
    if ( isdigit(ip) ) {
        ho = 50;
        (comparator) = intcmp;
    } else {
        (comparator) = stringcmp;
    }
    char * func = argv[1];

    if ( func[1] == 'q') {
        //printf("fii\n");
       quickSort(Box,(*comparator));
        //printf("We made it\n");
    } else if (func[1] == 'i') {
        //printf("fooo\n");
       insertionSort(Box,(*comparator));
    } else {
        printf("Fatal Error: '%s' is not a valid sort flag\n", func);
        exit(1);
    }
   //printf("**\n");
    printArr(Box->arr,Box->size,ip);
    //printf("**\n");
    if ( ho == 50 ) {
        //printf("I fired\n");
    }
    nodeDestroy(head);
    ddDestroy(Box);
    //free(temp);
    //free(temp3);
    free(mock);
    close(fd);
    return 0;

}
//-----------------------------------------------------------------------------
void ddDestroy(DD* Box) {

    int size = Box->size;

    int i;

    for ( i = 0; i < size; i++ ) {

        char* curr = Box->arr[i];

        free(curr);

    }

    free(Box->arr);

    free(Box);
}
//-----------------------------------------------------------------------------
void nodeDestroy(node* head) {

    node * tmp;

    while ( head != NULL ) {

        tmp = head;

        head = head->next;

        free(tmp->str);

        free(tmp);

    }

    head = NULL;
}
//-----------------------------------------------------------------------------
int matchCheck(node* a, char ip) {

    int ref = isdigit(ip);

    node* p = a;

    while ( p != NULL ) {

        int pef = isalpha(p->str[0]);

        int lef = isdigit(p->str[0]);

        if ( ref == 1 ) {

            if ( pef == 1 ) {

                return 0;

            }

        }

        if ( ref == 0 ) {

            if ( lef == 1 ) {

                return 0;

            }

        }

        p = p->next;

    }

    return 1;
}
//----------------------------------------------------------------------------
void printArr(char **A, int size, char ip) {

    int i = 0;

    int x,y;

    int z = isdigit(ip);

    while ( i < size ) {

        x = isdigit(A[i][0]);
        y = isalpha(A[i][0]);

        if ( x == 0 && y == 0 && z != 0) {

            printf("0\n");

        } else {

            printf("%s\n", A[i]);

        }
        i++;

    }
}
//-----------------------------------------------------------------------------
void swapper ( char ***a, char ***b) {

    char ** temp = *a;

    *a = *b;

    *b = temp;
}
//-----------------------------------------------------------------------------
int countMe(node* a) {

    int c = 0;

    node* p = a;

    while ( p != NULL ) {

        c++;

        p = p->next;
    }

    return c;
}
//-----------------------------------------------------------------------------
int insertionSort(void* toSort, int (*comparator) (void*, void*))
{
    DD* po = (DD*) toSort;
    char** A = po->arr;
    int j = 0;
    char* k;
    int i = 1;
    while ( i < po->size ) {

        k = A[i];

        j = i - 1;

        while ( j >= 0 && (((comparator)(A[j] ,k)) == -1) ) {

            A[j+1] = A[j];

            j = j-1;

        }

        swap(&(A[j+1]),&k);

        i++;

    }
}
//-----------------------------------------------------------------------------
int isItNumOrLet(char x) {
 //printf("i made it here\n");

    if (isalpha(x)) {
        return 1;
    }

    if (isdigit(x)) {
        return 1;
    }

    return 0;

}
//-----------------------------------------------------------------------------
node* insertLL(node* head, char* temp) {
    //printf("%s\n",temp);
    int a = strlen(temp);
    //printf("ITSSSSS %d\n",a);
    node* po = (node*) malloc(sizeof(node));

    swap(&(po->str),&(temp));
    po->prev = NULL;

    if ( head == NULL ) {
        head = po;
        return head;
    }

    node* z = head;

    while (z->next != NULL) {
        z = z->next;
    }

    z->next = po;
    po->prev = z;

    return head;

}
//-----------------------------------------------------------------------------
void printList(node* head) {
 node* p = head;
 while ( p != NULL ) {
  //printf("%s\n", p->str);
   if ( p->prev != NULL ) {
        //printf("prev is %s\n",p->prev->str);
    }
  p = p->next;
 }
}
//-----------------------------------------------------------------------------
int intcmp(void* int1, void* int2) { //(LEFTBIGGER -1   EQUAL 0     RIGHTBIGGER 1
    /*
    char *s1 = (char*) int1;
    char *s2 = (char*) int2;
    */
    long a = 0;
    long b = 0;

    char* s1 = (char*) int1;

    char* s2 = (char*) int2;

    int i;

    for ( i = 0; s1[i] != '\0'; i++ ) {

        a = a * 10 + s1[i] - '0';
    }

    for ( i = 0; s2[i] != '\0'; i++ ) {

        b = b * 10 + s2[i] - '0';
    }

    if ( a < 0 ) {
        a = a * -1;
        //printf("%s was made negative\n",s1);
    }

    if ( b < 0 ) {
        b = b * -1;
        //printf("%s was made negative\n",s2);
    }


    if ( a > b ) { //if the left parameter is bigger, return 1
        //printf("%d is bigger than %d\n",a,b);
        return -1;
    }

    if ( a == b ) {
        return 0;
        //printf("%d and %d are equal\n",a,b);
    }

    //printf("%d is smaller than %d\n",a,b);

    return 1;

}
//-----------------------------------------------------------------------------
int stringcmp(void* str1, void* str2) { //EVEN 0 LEFTBIGGER -1 RIGHTBIGGER 1

    char* s1 = (char *) str1;
    char* s2 = (char *) str2;

    int a = strlen(s1);
    int b = strlen(s2);

    int x;

    if ( a < b ) {
        x = a;
    } else {
        x = b;
    }

    int c = 0;
    //printf("%s\n",s1);
    //printf("%s\n",s2);


    while ( c < x ) {
        if (*(s1 + c) < *(s2 + c) ) {
            //printf("First is %c ",((char*)str1+c));
            //printf("second is %c ", ((char*)str2+c));
            //printf("I RETURNED 1 193\n");
            return 1;
        }

        if (*(s1 + c) > *(s2 + c)) {
            return -1;
        }

        c++;
    }

    if ( a == b ) {

        return 0;

    }

    if ( a == x ) {
        //printf("I RETURNED 1 205\n");
        return 1;
    }

    return -1;

}

//-----------------------------------------------------------------------------
void swap ( char** s1, char** s2 ) {

    char *temp = *s1;

    *s1 = *s2;

    *s2 = temp;
}
//-----------------------------------------------------------------------------
int quickSort( void* toSort, int (*comparator)(void*,void*)) {

    DD * F = (DD*) toSort;

    char** A = F->arr;

    recQuick(A,0,F->size-1,(comparator));

    return 1;
}
//-----------------------------------------------------------------------------
void recQuick(char **A, int s, int e, int (*comparator)(void*,void*)) {
    //s = (node*) toSort;

    //printf("end is %d\n",e);

    if ( s < e ) { //we want to cycle through the linked list

        int piv = realQuickSorter(A,s,e,(comparator));

        recQuick(A,s,piv-1,(comparator));

        recQuick(A,piv + 1, e, (comparator));
    }


    //return 1;

}
//-----------------------------------------------------------------------------
node* bringMeEnd(node* list) {

    node* temp = list;

    while ( list && list->next ) {
        list = list->next;
    }
    //printf("end is %s\n",list->str);
    return list;
}
//-----------------------------------------------------------------------------
int areEqual (char* s1, char* s2 ) {

    if ( isdigit(s1[0]) == 1 ) {

        int a = atoi(s1);

        int b = atoi(s2);

        if ( a == b ) {

            return 1;

        }

        return 0;

    }

    int x = strlen(s1);

    int y = strlen(s2);

    if ( x == y ) {

        int i = 0;

        while ( i < x ) {

            if ( s1[i] != s2[i] ) {

                return 0;

            }

            i++;

        }

    }

    return 0;
}
//-----------------------------------------------------------------------------

int realQuickSorter ( char** A, int s, int e , int (*comparator)(void*,void*)) {

    //printArr(A);

    int i = s + 1;

    char* piv = A[s];

    int j;

    for ( j = s + 1; j <= e; j++ ) {

        if ( ((comparator)(A[j],piv)) == 1 ) {

            swap(&(A[i]),&(A[j]));

            i++;
        }
    }

    swap(&(A[s]),&(A[i-1]));

    return i - 1;
}
