#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<conio.h>

struct books {
    int id;
    char bookName[50];
    char authorName[50];
    char date[12];
} b;

struct student {
    int id;
    char sName[50];
    char sClass[50];
    int sRoll;
    char bookName[50];
    char date[12];
} s;

FILE *fp;


void strip_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

// function prototypes
void addBook();
void booksList();
void del();
void issueBook();
void issueList();
void clearIssueList();   

int main() {

    int ch;

    while (1) {
        system("cls");
        printf("<== Library Management System ==>\n");
        printf("1. Add Book\n");
        printf("2. Books List\n");
        printf("3. Remove Book\n");
        printf("4. Issue Book\n");
        printf("5. Issued Book List\n");
        printf("6. Clear Issued Book List\n"); 
        printf("0. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);
        getchar();  

        switch (ch) {
        case 0:
            exit(0);

        case 1:
            addBook();
            break;

        case 2:
            booksList();
            break;

        case 3:
            del();
            break;

        case 4:
            issueBook();
            break;

        case 5:
            issueList();
            break;

        case 6:
            clearIssueList();   
            break;

        default:
            printf("Invalid Choice...\n\n");
        }

        printf("Press Any Key To Continue...");
        getch();
    }

    return 0;
}


// -----------------------------------
// Add Book
// -----------------------------------
void addBook() {
    char myDate[12];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(myDate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    strcpy(b.date, myDate);

    fp = fopen("books.txt", "ab");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter book id: ");
    scanf("%d", &b.id);
    getchar();

    printf("Enter book name: ");
    fgets(b.bookName, sizeof(b.bookName), stdin);
    strip_newline(b.bookName);

    printf("Enter author name: ");
    fgets(b.authorName, sizeof(b.authorName), stdin);
    strip_newline(b.authorName);

    fwrite(&b, sizeof(b), 1, fp);
    fclose(fp);

    printf("Book Added Successfully\n");
}


// -----------------------------------
// Books List
// -----------------------------------
void booksList() {

    system("cls");
    printf("<== Available Books ==>\n\n");
    printf("%-10s %-30s %-25s %s\n\n",
           "Book ID", "Book Name", "Author", "Date");

    fp = fopen("books.txt", "rb");
    if (fp == NULL) {
        printf("No books found!\n");
        return;
    }

    while (fread(&b, sizeof(b), 1, fp) == 1) {
        printf("%-10d %-30s %-25s %s\n",
               b.id, b.bookName, b.authorName, b.date);
    }

    fclose(fp);
}


// -----------------------------------
// Delete Book
// -----------------------------------
void del() {
    int id, found = 0;
    system("cls");
    printf("<== Remove Books ==>\n\n");
    printf("Enter Book id to remove: ");
    scanf("%d", &id);

    FILE *ft;

    fp = fopen("books.txt", "rb");
    if (fp == NULL) {
        printf("No books to delete!\n");
        return;
    }
    ft = fopen("temp.txt", "wb");

    while (fread(&b, sizeof(b), 1, fp) == 1) {
        if (id == b.id) {
            found = 1;
        } else {
            fwrite(&b, sizeof(b), 1, ft);
        }
    }

    fclose(fp);
    fclose(ft);

    remove("books.txt");
    rename("temp.txt", "books.txt");

    if (found)
        printf("\n\nDeleted Successfully.");
    else
        printf("\n\nRecord Not Found !");
}



// -----------------------------------
// Issue Book
// -----------------------------------
void issueBook() {

    char myDate[12];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(myDate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    strcpy(s.date, myDate);

    int found = 0;

    system("cls");
    printf("<== Issue Books ==>\n\n");

    printf("Enter Book id to issue: ");
    scanf("%d", &s.id);
    getchar();

    fp = fopen("books.txt", "rb");
    if (fp == NULL) {
        printf("No books available!\n");
        return;
    }

    while (fread(&b, sizeof(b), 1, fp) == 1) {
        if (b.id == s.id) {
            strcpy(s.bookName, b.bookName);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found) {
        printf("No book found with this id\n");
        return;
    }

    fp = fopen("issue.txt", "ab");

    printf("Enter Student Name: ");
    fgets(s.sName, sizeof(s.sName), stdin);
    strip_newline(s.sName);

    printf("Enter Student Class: ");
    fgets(s.sClass, sizeof(s.sClass), stdin);
    strip_newline(s.sClass);

    printf("Enter Student Roll: ");
    scanf("%d", &s.sRoll);

    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);

    printf("Book Issued Successfully\n\n");
}


// -----------------------------------
// Issue List
// -----------------------------------
void issueList() {

    system("cls");
    printf("<== Book Issue List ==>\n\n");

    printf("%-10s %-30s %-15s %-12s %-35s %-12s\n\n",
           "S.ID", "Name", "Class", "Roll", "Book Name", "Date");

    fp = fopen("issue.txt", "rb");
    if (fp == NULL) {
        printf("No issued books!\n");
        return;
    }

    while (fread(&s, sizeof(s), 1, fp) == 1) {
        printf("%-10d %-30s %-15s %-12d %-35s %-12s\n",
               s.id, s.sName, s.sClass, s.sRoll, s.bookName, s.date);
    }

    fclose(fp);
}



// -----------------------------------
// Clear Issued Book List (NEW FEATURE)
// -----------------------------------
void clearIssueList() {
    fp = fopen("issue.txt", "wb"); // opening in write mode clears file
    if (fp == NULL) {
        printf("Unable to clear issued list!\n");
        return;
    }
    fclose(fp);
    printf("Issued Book List Cleared Successfully!\n");
}

