#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define max_length 65000

typedef struct node
{
    char task[max_length];
    int num;
    struct node *next;
}
node;

void help(void);                 // calls different functions
void add(char* taskadd);
void list(void);
void reverseprint(struct node *p);
void rem (char* taskrm);
void done (char* taskrm);
void count(void);



int comp = 0; // global variable for keeping track of number of completed task
int pen = 0; // global variable for keeping track of number of pending task
int flag; // acts like a boolean flag  for controlling if statement
char *ch1;


int main(int argc, char* argv[])
{
    if(argc == 1 || strcasecmp(argv[1],"help") == 0)
    {
        help(); //calls the help function which prints on the terminal on how to use the program
        exit(0);
    }

    char* task= argv[2];

    if(strcasecmp(argv[1],"add") == 0 )
    {
        if(argc < 3)
        {
            printf("Error: Missing todo string. Nothing added!\n");
            exit(0);
        }

        add(task);
    }

    if(strcasecmp(argv[1],"ls") == 0)
    {

        count(); // calls count function for recounting number of tasks left.
        if(pen == 0) // if number of task left is zero then this if statement is executed.
        {
            printf("There are no pending todos!\n");
            exit(0);
        }

        list(); // if number of task left are not equal to zero then it calls the list function

    }

    if(strcasecmp(argv[1],"del") == 0)
    {
        if(argc < 3)
        {
            printf("Error: Missing NUMBER for deleting todo.\n");
            exit(0);
        }
        count();
        rem(task);
    }
    if(strcasecmp(argv[1],"done") == 0)
    {
        if(argc < 3)
        {
            printf("Error: Missing NUMBER for marking todo as done.\n");
            exit(0);
        }
        count();
        done(task);
    }
    if(strcasecmp(argv[1],"report") == 0)
    {

        time_t t;    struct tm* tm;             // a struct for formatting date according to spec.
        char Date[11];
        time(&t);
        tm = localtime(&t);
        strftime(Date, sizeof Date, "%Y-%m-%d", tm);
        count();                                // call count to update variable pen and comp.
        printf("%s Pending : %i Completed : %i\n",Date,pen,comp);

    }

    return 0;

}


void help(void) // prints how to use the program when ./todo help is run or ./todo.
{
    printf("Usage :-\n\
$ ./todo add \"todo item\"  # Add a new todo\n\
$ ./todo ls               # Show remaining todos\n\
$ ./todo del NUMBER       # Delete a todo\n\
$ ./todo done NUMBER      # Complete a todo\n\
$ ./todo help             # Show usage\n\
$ ./todo report           # Statistics\n");
    exit(0);

}


void add (char* taskadd) // adds task to todo list.
{

    FILE *file = fopen("todo.txt", "a");
    if (file == NULL)
    {
        exit(0);
    }
    fprintf(file, "%s\n", taskadd);
    printf("Added todo: \"%s\"\n",taskadd);


    fclose(file);
}


void list(void) // prints the remaining task in a reverse order.
{


    FILE *file = fopen("todo.txt", "r");

    if (file == NULL)
    {
        exit(0);
    }

    char temp [max_length];
    node *head = malloc(sizeof(node)); // mallocing for linked list head

    int i = 1; // counter for assigning order to task as they are added.

    while(fgets(temp,max_length,file))
    {
        node *n = malloc(sizeof(node));
        node *tempn = malloc(sizeof(node));
        strcpy(n -> task,temp); // strcpy copies from temp variable to a node's (task) field.
        n -> num = i;           // for assigning order to the same node.
        i++;
        if (head == NULL)
        {
            head = n;
        }
        else
        {
            tempn = head;

            while(tempn -> next != NULL)
            {
                tempn = tempn -> next;
            }

            tempn -> next = n;
        }
    }
    fclose(file);


    reverseprint(head -> next);

}

void reverseprint(struct node *p) // prints the entire linked list in reverse order without reversing the linked list.
{
    if(p == NULL)
    {
        return;
    }
    reverseprint(p->next);

    printf("[%i] %s", p->num , p->task);
    


}

void rem (char* taskrm)
{

    int lno = atoi(taskrm); // converts string to int for easy manipulation.
    if(lno < 1 || lno > pen) // checks if the user has inputted invalid number for removal.
    {
        printf("Error: todo #%i does not exist. Nothing deleted.\n",lno);
        exit(0);
    }
    int ctr = 0;
    char str[max_length];

    FILE *file = fopen("todo.txt", "r");

    if (file == NULL)
    {

        exit(0);
    }
    FILE *filet = fopen("temp.txt","w");
    if (filet == NULL)
	{

        exit (0);
    }
    while(!feof(file))
    {
        strcpy(str,"\0");
        fgets(str,max_length,file);
        if(!feof(file))
        {
            ctr++;
            if(ctr != lno)
            {
                fprintf(filet,"%s",str);
            }
        }
    }
    fclose(file);
    fclose(filet);
    remove("todo.txt");
    rename("temp.txt","todo.txt");
    if(flag != 1) // prevents the printing of statement inside  when called by done().
    {
        printf("Deleted todo #%i\n",lno);
    }
    flag = 0;


}

void done (char* taskrm)
{


    int lno = atoi(taskrm);
    if(lno < 1 || lno > pen) // checks if the user has inputted invalid number for removal.
    {
        printf("Error: todo #%i does not exist.\n",lno);
        exit(0);
    }
    int ctr = 0;
    char str[max_length];

    FILE *file = fopen("todo.txt", "r");

    if (file == NULL)
    {

        exit(0);
    }
    FILE *filet = fopen("done.txt","a");
    if (filet == NULL)
	{
        exit(0);

    }
    while(!feof(file))
    {
        strcpy(str,"\0");
        fgets(str,max_length,file);
        if(!feof(file))
        {
            ctr++;
            if(ctr == lno)
            {
                time_t t;    struct tm* tm;
                char Date[11];
                time(&t);
                tm = localtime(&t);
                strftime(Date, sizeof Date, "%Y-%m-%d", tm);
                fprintf(filet,"x %s ",Date); // prints x and then current date to .txt file.
                fprintf(filet,"%s",str);
            }
        }
    }
    fclose(file);
    fclose(filet);
    flag = 1;
    printf("Marked todo #%i as done.\n",lno);

    rem(taskrm);

}

void count(void) // for counting number of todo's left.
{
    char ch;
    FILE *filet = fopen("todo.txt", "r");
    if(filet == NULL)
    {
        return;
    }
    while((ch = fgetc(filet)) != EOF)
    {
        if(ch == '\n')
            pen++;
    }
    fclose (filet);

    FILE *filec = fopen("done.txt", "r");
    if(filec == NULL)
    {
        comp = 0;
        return;
    }
    while((ch = fgetc(filec)) != EOF)
    {
        if(ch == '\n')
            comp++;
    }
    fclose (filec);
}
