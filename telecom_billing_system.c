#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX_PHONE_LENGTH 20
#define MAX_NAME_LENGTH 50
#define FILE_PATH "file.ojs"

struct subscriber
{
    char phonenumber[MAX_PHONE_LENGTH];
    char name[MAX_NAME_LENGTH];
    float amount;
};

void addrecords();
void listrecords();
void modifyrecords();
void deleterecords();
void searchrecords();
void payment();
void clearScreen();

int main()
{
    char choice;

    clearScreen();

    printf("\n\n\n\n\n\n\n\n\n**************************************************************");
    printf("\n\t\t------WELCOME TO THE TELECOM BILLING MANAGEMENT SYSTEM---");
    printf("\n\t\t****************************************************************");
    printf("\n\n\nPress any key to continue...");
    getchar();
    clearScreen();

    while (1)
    {
        clearScreen();
        printf("\n Enter\n A : for adding new records.\n L : for list of records");
        printf("\n M : for modifying records.\n P : for payment");
        printf("\n S : for searching records.\n D : for deleting records.\n E : for exit\n");
        choice = getchar();
        choice = toupper(choice);
        switch (choice)
        {
        case 'P':
            payment();
            break;
        case 'A':
            addrecords();
            break;
        case 'L':
            listrecords();
            break;
        case 'M':
            modifyrecords();
            break;
        case 'S':
            searchrecords();
            break;
        case 'D':
            deleterecords();
            break;
        case 'E':
            printf("\n\n\t\t\t\tTHANK YOU");
            printf("\n\n\n\n\n:\n\tFOR USING OUR SERVICE\n");
            return 0;
        default:
            printf("Incorrect Input\n");
            printf("Press any key to continue...");
            getchar();
        }
    }
}

void addrecords()
{
    FILE *f;
    struct subscriber s;

    f = fopen(FILE_PATH, "ab+");
    if (f == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    while (1)
    {
        clearScreen();
        printf("\n Enter phone number:");
        scanf("%s", s.phonenumber);
        printf("\n Enter name:");
        fflush(stdin);
        scanf("%s", s.name); // Changed %[^\n] to %s for name input
        printf("\n Enter amount:");
        scanf("%f", &s.amount);
        fwrite(&s, sizeof(s), 1, f);
        fflush(stdin);
        clearScreen();
        printf("1 record successfully added");
        printf("\n Press esc key to exit, any other key to add other record:");
        if (getchar() == 27) // Changed getche() to getchar() for better input handling
            break;
    }

    fclose(f);
}

void listrecords()
{
    FILE *f;
    struct subscriber s;
    int i;

    f = fopen(FILE_PATH, "rb");
    if (f == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    clearScreen();
    printf("Phone Number\t\tUser Name\t\t\tAmount\n");
    for (i = 0; i < 79; i++)
        printf("-");
    printf("\n");

    while (fread(&s, sizeof(s), 1, f) == 1)
    {
        printf("%-10s\t\t%-20s\t\tRs. %.2f /-\n", s.phonenumber, s.name, s.amount);
    }

    fclose(f);
    printf("\nPress any key to continue...");
    getchar();
}

void deleterecords()
{
    FILE *f, *t;
    struct subscriber s;
    int found = 0;
    char phonenumber[MAX_PHONE_LENGTH];

    f = fopen(FILE_PATH, "rb");
    if (f == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    t = fopen("temp.ojs", "wb");
    if (t == NULL)
    {
        fclose(f);
        printf("Error opening temporary file.\n");
        return;
    }

    clearScreen();
    printf("Enter the phone number to be deleted from the Database: ");
    scanf("%s", phonenumber);

    while (fread(&s, sizeof(s), 1, f) == 1)
    {
        if (strcmp(s.phonenumber, phonenumber) == 0)
        {
            found = 1;
            continue;
        }
        fwrite(&s, sizeof(s), 1, t);
    }

    fclose(f);
    fclose(t);

    if (found == 0)
    {
        clearScreen();
        printf("Phone number \"%s\" not found.\n", phonenumber);
        printf("\nPress any key to continue...");
        getchar();
        return;
    }

    remove(FILE_PATH);
    rename("temp.ojs", FILE_PATH);

    clearScreen();
    printf("The Number %s Successfully Deleted!!!!\n", phonenumber);
    printf("\nPress any key to continue...");
    getchar();
}

void searchrecords()
{
    FILE *f;
    struct subscriber s;
    char phonenumber[MAX_PHONE_LENGTH];
    int found = 0;

    f = fopen(FILE_PATH, "rb");
    if (f == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    clearScreen();
    printf("Enter Phone Number to search in our database: ");
    scanf("%s", phonenumber);

    while (fread(&s, sizeof(s), 1, f) == 1)
    {
        if (strcmp(s.phonenumber, phonenumber) == 0)
        {
            clearScreen();
            printf("Record Found\n");
            printf("\nPhonenumber: %s\nName: %s\nAmount: Rs.%0.2f\n", s.phonenumber, s.name, s.amount);
            found = 1;
            break;
        }
    }

    fclose(f);

    if (found == 0)
    {
        clearScreen();
        printf("Requested Phone Number Not found in our database.\n");
    }

    printf("\nPress any key to continue...");
    getchar();
}

void modifyrecords()
{
    FILE *f;
    struct subscriber s;
    char phonenumber[MAX_PHONE_LENGTH];
    int found = 0;

    f = fopen(FILE_PATH, "rb+");
    if (f == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    clearScreen();
    printf("Enter phone number of the subscriber to modify: ");
    scanf("%s", phonenumber);

    while (fread(&s, sizeof(s), 1, f) == 1)
    {
        if (strcmp(s.phonenumber, phonenumber) == 0)
        {
            clearScreen();
            printf("\nEnter new phone number: ");
            scanf("%s", s.phonenumber);
            printf("\nEnter new name: ");
            scanf("%s", s.name); // Changed %[^\n] to %s for name input
            printf("\nEnter new amount: ");
            scanf("%f", &s.amount);

            fseek(f, -sizeof(s), SEEK_CUR);
            fwrite(&s, sizeof(s), 1, f);
            found = 1;
            break;
        }
    }

    fclose(f);

    if (found == 0)
    {
        clearScreen();
        printf("Phone number \"%s\" not found.\n", phonenumber);
    }

    printf("\nPress any key to continue...");
    getchar();
}

void payment()
{
    FILE *f;
    struct subscriber s;
    char phonenumber[MAX_PHONE_LENGTH];
    float amt;
    int found = 0;

    f = fopen(FILE_PATH, "rb+");
    if (f == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    clearScreen();
    printf("Enter phone number of the subscriber for payment: ");
    scanf("%s", phonenumber);

    while (fread(&s, sizeof(s), 1, f) == 1)
    {
        if (strcmp(s.phonenumber, phonenumber) == 0)
        {
            clearScreen();
            printf("Phone No.: %s\n", s.phonenumber);
            printf("Name: %s\n", s.name);
            printf("Current amount: %.2f\n", s.amount);
            printf("\nEnter amount of payment: ");
            scanf("%f", &amt);

            s.amount -= amt;
            fseek(f, -sizeof(s), SEEK_CUR);
            fwrite(&s, sizeof(s), 1, f);
            found = 1;
            break;
        }
    }

    fclose(f);

    if (found == 0)
    {
        clearScreen();
        printf("Phone number \"%s\" not found.\n", phonenumber);
    }
    else
    {
        clearScreen();
        printf("THANK YOU %s FOR YOUR TIMELY PAYMENTS\n", s.name);
    }

    printf("\nPress any key to continue...");
    getchar();
}

void clearScreen()
{
    printf("\033[2J\033[1;1H"); // ANSI escape code to clear screen
}
