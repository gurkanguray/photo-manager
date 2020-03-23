/* 
    Owner: Güray Gürkan
    Code Editor: Visual Studio Code
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_NAME 15
#define MAX_DIM 10
#define MAX_SIZE 10
#define MAX_LOCATION 50

struct Time //structure definition for date & time
{
    int day;
    int month;
    int year;
    int hour;
    int minute;
};

struct Photo //structure definition for Node
{
    int id; //max 5 digit
    char name[MAX_NAME];
    char dimension[MAX_DIM];
    char size[MAX_SIZE];
    char city[MAX_LOCATION];
    char country[MAX_LOCATION];
    struct Time datetime;
    struct Photo *next;
};
typedef struct Photo *Photos;

struct LinkedList //structure definition for LinkedList
{
    struct Photo *head;
    struct Photo *tail;
    int size;
};
typedef struct LinkedList *List;

int menu(void);
Photos *emptyList(void);
List *createList(void);
struct Time splitdateTime(struct Time, char *);
List *initialisePhotosInfo(struct LinkedList *);
List *addPhotoInfo(struct LinkedList *, char *, char *, char *, char *, char *);
void printPhotosInfo(struct LinkedList *);
void searchPhotosInfo(struct LinkedList *);
List *deletePhotoInfo(struct LinkedList *, struct LinkedList *, int);
void printRecentlyDeletedPhotoInfo(struct LinkedList *);

int main() //main
{
    struct LinkedList *photo, *deletedPhoto;
    int choice, photoID;
    char name[MAX_NAME], dimension[MAX_DIM], size[MAX_SIZE], city[MAX_LOCATION], country[MAX_LOCATION];

    deletedPhoto = createList();
    photo = createList();
    photo = initialisePhotosInfo(photo);

    do
    {
        choice = menu();
        switch (choice)
        {
        case 1:
            printf("\n\nEnter name of the photo: ");
            scanf("%s", name);
            printf("Enter dimension of the photo: ");
            scanf("%s", dimension);
            printf("Enter size of the photo: ");
            scanf("%s", size);
            printf("Enter name of the city: ");
            scanf("%s", city);
            printf("Enter name of the country: ");
            scanf("%s", country);

            addPhotoInfo(photo, name, dimension, size, city, country);
            break;
        case 2:
        {
            printPhotosInfo(photo);
            break;
        }
        case 3:
        {
            searchPhotosInfo(photo);
            break;
        }
        case 4:
            printf("\nEnter the ID of the Photo you want to delete: ");
            scanf("%d", &photoID);
            deletePhotoInfo(photo, deletedPhoto, photoID);
            break;
        case 5:
            printRecentlyDeletedPhotoInfo(deletedPhoto);
            break;
        case 6:
            printf("Goodbye!!!\n");
            break;
        default:
            printf("Wrong option, Please try again\n");
        }
    } while (choice != 6);

    return 0;
}

int menu() //The menu which returns to an integer according the input.
{
    int option;
    printf("----- MENU -----\n");
    printf("1. Add a new photo\n");
    printf("2. Print Photos\n");
    printf("3. Search Photos\n");
    printf("4. Delete Photos\n");
    printf("5. Print recently deleted photos\n");
    printf("6. Exit\n\n");
    printf("Enter your option: ");
    scanf("%d", &option);

    return option;
}

Photos *emptyList() //creates dummy node
{
    struct Photo *temp;
    temp = (Photos *)malloc(sizeof(struct Photo));

    if (temp == NULL) //checks if memory allocated or not
    {
        printf("Error while allocating Photo");
        return NULL;
    }
    temp->id = 0;
    strcpy(temp->name, "");
    strcpy(temp->dimension, "");
    strcpy(temp->size, "");
    strcpy(temp->city, "");
    strcpy(temp->country, "");
    temp->next = NULL;

    temp->datetime.day = 0;
    temp->datetime.month = 0;
    temp->datetime.year = 0;
    temp->datetime.hour = 0;
    temp->datetime.minute = 0;

    return temp;
}

List *createList() //creates LinkedList
{
    struct LinkedList *temp;
    temp = (List *)malloc(sizeof(struct LinkedList));

    if (temp == NULL) //checks if memory allocated or not
    {
        printf("Error while allocating linkedlist\n");
        return NULL;
    }
    temp->head = emptyList();
    temp->tail = temp->head;
    temp->size = 0;

    return temp;
}

struct Time splitdateTime(struct Time t, char *token) //Parses the date & time in order to write to the structure
{

    int i, isDateFinished = 0;
    int counter = 0, power = 1;
    for (i = 0; i < strlen(token); i++)
    {
        if (token[i] != '/' && token[i] != ' ' && token[i] != ':')
        { //parsing according to the delimeter
            if (isDateFinished == 0)
            {
                if (counter == 0) //parsing the days,months & years
                    t.day += (token[i] - 48) * pow(10, power--);
                else if (counter == 1)
                    t.month += (token[i] - 48) * pow(10, power--);
                else
                    t.year += (token[i] - 48) * pow(10, power--);
            }
            else
            {
                if (counter == 0) //parsing the hours & minutes
                    t.hour += (token[i] - 48) * pow(10, power--);
                else
                    t.minute += (token[i] - 48) * pow(10, power--);
            }
        }
        else if (token[i] == ' ')
        {
            isDateFinished = 1;
            counter = 0;
            power = 1;
        }
        else
        { // in order to write values in the correct form
            counter += 1;
            if (counter == 2)
                power = 3;
            else
                power = 1;
        }
    }
    return t;
}

List *initialisePhotosInfo(struct LinkedList *txt) //reads the PhotoInfoBook.txt file
{
    char ch, *token, str[200];
    int counter = 0;

    struct Photo *temp;
    temp = txt->head;

    FILE *input; //creates file stream
    input = fopen("PhotoInfoBook.txt", "r");
    while (fgets(str, sizeof(str), input)) //puts the read values to structure according to delimeter
    {
        temp->next = (struct Photo *)malloc(sizeof(struct Photo));
        temp = temp->next;

        token = strtok(str, ";");
        temp->id = token[0] - 48;
        while (token != NULL)
        {
            token = strtok(NULL, ";");
            if (counter == 0)
            {
                strcpy(temp->name, token);
            }
            else if (counter == 1)
            {
                strcpy(temp->dimension, token);
            }
            else if (counter == 2)
            {
                strcpy(temp->size, token);
            }
            else if (counter == 3)
            {
                strcpy(temp->city, token);
            }
            else if (counter == 4)
            {
                strcpy(temp->country, token);
            }
            else if (counter == 5)
            {
                token = strtok(token, "\n");
                temp->datetime = splitdateTime(temp->datetime, token);
            }
            else
            {
            }
            counter++;
        }
        counter = 0;
        txt->size++;
    }
    txt->tail = temp;
    fclose(input);

    return txt;
}

List *addPhotoInfo(struct LinkedList *add, char *name, char *dimension, char *size, char *city, char *country) //adds new photo details
{
    struct Photo *temp;
    temp = add->head;

    time_t rawtime = time(NULL);

    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = (struct Photo *)malloc(sizeof(struct Photo));
    temp = temp->next;
    temp->next = NULL;

    strcpy(temp->name, name);
    strcpy(temp->dimension, dimension);
    strcpy(temp->size, size);
    strcpy(temp->city, city);
    strcpy(temp->country, country);

    struct tm t = *localtime(&rawtime);
    temp->datetime.day = t.tm_mday;
    temp->datetime.month = t.tm_mon + 1;
    temp->datetime.year = t.tm_year + 1900;
    temp->datetime.hour = t.tm_hour;
    temp->datetime.minute = t.tm_min;

    printf("The Photo has been added!!!\n\n");

    temp->id = add->tail->id + 1;
    add->tail = temp;
    add->size += 1;

    return add;
}

void printPhotosInfo(struct LinkedList *p) //prints all photo details
{
    struct Photo *temp;
    temp = p->head;

    printf("\nPhotos in your database\n");
    printf("---------------------------\n");
    while (temp->next != NULL)
    {
        temp = temp->next;
        printf("ID: %d\n", temp->id);
        printf("Photo Name: %s\n", temp->name);
        printf("Photo Dimension: %s\n", temp->dimension);
        printf("Photo Size: %s\n", temp->size);
        printf("City Name: %s\n", temp->city);
        printf("Country Name: %s\n", temp->country);
        printf("Date: %02d/%02d/%d\n", temp->datetime.day, temp->datetime.month, temp->datetime.year);
        printf("Time: %02d:%02d\n", temp->datetime.hour, temp->datetime.minute);
    }
    printf("\n");
}

void searchPhotosInfo(struct LinkedList *p) //search the photos according to keyword
{
    struct Photo *temp;
    int option, i = 0, cmp_check;
    char keyword[MAX_LOCATION];

    do
    {
        printf("\n-----Searching By-----\n");
        printf("(1) Country Name\n");
        printf("(2) City Name\n");
        printf("Enter search by option: ");
        scanf("%d", &option);

        if (option != 1 && option != 2)
        {
            printf("Invalid option\n");
        }
    } while (option != 1 && option != 2);

    temp = p->head;
    (option == 1) ? printf("Enter country name: ") : printf("Enter city name: ");
    scanf("%s", keyword);
    while (temp->next != NULL)
    {
        temp = temp->next;
        if (option == 1)
        {
            //todo: key sensitive ?
            if (strcmp(temp->country, keyword) == 0) //checks if there is a match
            {
                displayResultsForSearch(temp); //prints the photo details
            }
            else if (temp->next == NULL && strcmp(temp->country, keyword))
            {
                printf("There is no country name with '%s' in the list, try to start with upper character\n\n", keyword);
            }
        }
        else if (option == 2)
        {

            if (strcmp(temp->city, keyword) == 0) //checks if there is a match
            {
                displayResultsForSearch(temp); //prints the photo details
            }
            else if (temp->next == NULL && strcmp(temp->city, keyword))
            {
                printf("There is no city name with '%s' in the list, try to start with upper character\n\n", keyword);
            }
        }
        else
        {
            printf("Invalid Option\n");
        }
    }
}

List *deletePhotoInfo(struct LinkedList *photo, struct LinkedList *deleted, int key) //deletes photo from the list
{
    struct Photo *deletedPhotos;
    deletedPhotos = deleted->head;
    while (deletedPhotos->next != NULL)
    {
        deletedPhotos = deletedPhotos->next;
    }
    deletedPhotos->next = (struct Photo *)malloc(sizeof(struct Photo));
    deletedPhotos = deletedPhotos->next;
    deletedPhotos->next = NULL;

    struct Photo *current, *temp;
    temp = photo->head;

    while (temp->next != NULL && temp->next->id != key) //checks if the photo exists or not
    {
        temp = temp->next;
    }
    if (temp->next != NULL) //checks the photo last or not
    {
        if (temp->next->next == NULL)
        {
            current = temp->next;
            temp->next = NULL;
            photo->tail = temp;
            CopyList(current, deletedPhotos);
            deleted->head = deletedPhotos;
            deleted->size += 1;
        }
        else
        {
            current = temp->next;
            temp->next = temp->next->next;
            current->next = NULL;
            CopyList(current, deletedPhotos);
            deleted->head = deletedPhotos;
            deleted->size += 1;
        }
        free(current);
        photo->size -= 1;
        printf("Photo with ID %d has been deleted from your list AND added to recently deleted list\n", key);
    }
    else
    {
        printf("No photo with ID %d in your list\n\n", key);
    }

    return photo;
}

void printRecentlyDeletedPhotoInfo(struct LinkedList *deleted) //prints the most recently deleted photo. Assignment description was not clear so assume like that.
{
    struct Photo *deletedPhoto;
    deletedPhoto = deleted->head;
    deletedPhoto->next = NULL;

    printf("ID: %d\n", deletedPhoto->id);
    printf("Photo Name: %s\n", deletedPhoto->name);
    printf("Photo Dimension: %s\n", deletedPhoto->dimension);
    printf("Photo Size: %s\n", deletedPhoto->size);
    printf("City Name: %s\n", deletedPhoto->city);
    printf("Country Name: %s\n", deletedPhoto->country);
    printf("Date: %02d/%02d/%d\n", deletedPhoto->datetime.day, deletedPhoto->datetime.month, deletedPhoto->datetime.year);
    printf("Time: %02d:%02d\n", deletedPhoto->datetime.hour, deletedPhoto->datetime.minute);
    printf("\n");
}

void CopyList(struct Photo *source, struct Photo *target) //copies a list from source to target in order to prevent code repition
{
    target->id = source->id;
    strcpy(target->name, source->name);
    strcpy(target->dimension, source->dimension);
    strcpy(target->size, source->size);
    strcpy(target->city, source->city);
    strcpy(target->country, source->country);
    target->datetime.day = source->datetime.day;
    target->datetime.hour = source->datetime.hour;
    target->datetime.minute = source->datetime.minute;
    target->datetime.month = source->datetime.month;
    target->datetime.year = source->datetime.year;
}

void displayResultsForSearch(struct Photo *match) //in order to prevent code repition in Search function, I splitted display part of Search function as a new function.
{
    printf("-------Results-------\n");
    printf("ID: %d\n", match->id);
    printf("Photo Name: %s\n", match->name);
    printf("Photo Dimension: %s\n", match->city);
    printf("Photo Size: %s\n", match->size);
    printf("City Name: %s\n", match->city);
    printf("Country Name: %s\n", match->country);
    printf("Date: %02d/%02d/%d\n", match->datetime.day, match->datetime.month, match->datetime.year);
    printf("Time: %02d:%02d\n", match->datetime.hour, match->datetime.minute);
}
