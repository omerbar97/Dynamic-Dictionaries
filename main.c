/***********************
* Name: Omer Bar
* ID: 315318766
* Assignment: ex_5
***********************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIRST 0
#define NEW_DICTIONARY 1
#define NEW_WORD 2
#define DELETE_WORD 3
#define SEARCH_WORD 4
#define DELETE_DICTIONARY 5
#define EXIT 6
#define SPACE_STRING " "
#define SPACE_CHAR ' '
#define COMMA ','
#define ENTER '\n'
#define END_STRING "\0"
#define END_STRING_CHAR '\0'
#define YES 'y'

///STRUCTS
typedef struct Word {
    char** translations;
    struct Word* next;
} Word;

typedef struct {
    char** languages;
    int numOfLanguages;
    Word* wordList;
} Dictionary;

///function declaration
char** stringToArray(int* numOfLanguages, int* flagOfMalloc);
char* endlessINPUT(int *flagOfMalloc);
void printMenu();
void insertToEnd(Word** root, int *flagOfMalloc, int numOflanguages);
void delete_Word(Word **root, char *strToDelete, int *WrongInput, int sizeOfArray);
void search_Word(char* strToSearch, Dictionary *dictionary);
void free_Dictionary(Dictionary *dictionary, int sizeOfLanguages);
void swapDictionary(Dictionary* First, Dictionary* Second);

int main()
{
    ///variable declaration
    Dictionary* dictionaries = NULL;
    Dictionary* temp = NULL;
    int numOfDictionaries = 0, userChoice = 0, isMemoryDictionaryExtra = 0, flagMenu = 1;
    ///while for menu until userChoice == 6
    while(userChoice != EXIT)
    {
        if(flagMenu == 1)
            printMenu(); //print the menu
        scanf("%d", &userChoice);
        switch(userChoice)
        {
            ///create new dictionary
            case NEW_DICTIONARY:
            {
                ///variable declaration
                int flagOfMalloc = 0, numOflanguages = 0;
                printf("Define a new dictionary:\n");
                if(numOfDictionaries == 0 && isMemoryDictionaryExtra == 0)//first time creating a dictionary
                {
                    dictionaries = (Dictionary*)malloc(sizeof(Dictionary)*(numOfDictionaries + 1));
                    if(dictionaries == NULL)
                    {
                        printf("The creation of the dictionary has failed!\n");
                        break;
                    }
                }
                else if(numOfDictionaries > 0 && isMemoryDictionaryExtra == 0)
                {
                    temp = dictionaries;//temp to hold the address while realloc
                    dictionaries = (Dictionary*)realloc(dictionaries, sizeof(Dictionary)*(numOfDictionaries + 1));
                    if(dictionaries == NULL)
                    {
                        dictionaries = temp;
                        printf("The creation of the dictionary has failed!\n");
                        break;
                    }
                }
                if(isMemoryDictionaryExtra == 1)//when it failed to realloc a block down there is already a memory block available
                {
                    dictionaries[numOfDictionaries + 1].languages = stringToArray(&numOflanguages, &flagOfMalloc);
                    if(dictionaries[numOfDictionaries + 1].languages == NULL)
                    {
                        //IN CASE the dictionaries[numOfDictionaries + 1].languages = NULL we need to free one block of
                        //dictionary at the end.
                        temp = dictionaries;
                        dictionaries = realloc(dictionaries, sizeof(Dictionary)*numOfDictionaries);//one block down
                        if(dictionaries == NULL)
                        {
                            dictionaries = temp; // now the dictionary is still with 1 block extra from before!
                            isMemoryDictionaryExtra = 1;//indicate if I have failed to free the extra block
                            dictionaries[numOfDictionaries].numOfLanguages = 0; // if the user want to exit for free memory.
                            temp = NULL;
                        }
                        printf("The creation of the dictionary has failed!\n");
                        break;
                    }
                    else
                    {
                        ///the memory block allocation was succeeded
                        dictionaries[numOfDictionaries + 1].numOfLanguages = numOflanguages;
                        dictionaries[numOfDictionaries + 1].wordList = NULL;
                        printf("The dictionary has been created successfully!\n");
                        isMemoryDictionaryExtra = 0;
                        numOfDictionaries++;
                        flagMenu = 1;
                        break;
                    }
                }
                else
                {
                    dictionaries[numOfDictionaries].languages = stringToArray(&numOflanguages, &flagOfMalloc);
                    if(dictionaries[numOfDictionaries].languages == NULL)
                    {
                        //IN CASE the dictionaries[numOfDictionaries].languages = NULL we need to free one block of
                        //dictionary at the end.
                        temp = dictionaries;
                        dictionaries = realloc(dictionaries, sizeof(Dictionary)*numOfDictionaries);//one block down
                        if(dictionaries == NULL)
                        {
                            dictionaries = temp; // now the dictionary is still with 1 block extra from before!
                            isMemoryDictionaryExtra = 1;//indicate if I have failed to free the extra block
                            dictionaries[numOfDictionaries].numOfLanguages = 0; // if the user want to exit for free memory.
                            temp = NULL;
                        }
                        printf("The creation of the dictionary has failed!\n");
                        break;
                    }
                    else
                    {
                        ///the memory block allocation was succeeded
                        dictionaries[numOfDictionaries].numOfLanguages = numOflanguages;
                        dictionaries[numOfDictionaries].wordList = NULL;
                        printf("The dictionary has been created successfully!\n");
                        isMemoryDictionaryExtra = 0;
                        numOfDictionaries++;
                        flagMenu = 1;
                        break;
                    }
                }
            }
                ///add a new translation to dictionary
            case NEW_WORD:
            {
                ///when there is no dictionary the option not available
                if(numOfDictionaries == 0)
                {
                    printf("This option is not available right now, try again:\n");
                    flagMenu = 0;
                    break;
                }
                flagMenu = 1;
                int input = 0, flagOfMalloc;
                ///print the dictionary's
                printf("Choose a dictionary:\n");
                for(int i = 0; i < numOfDictionaries; i++)
                {
                    printf("%d. ", i + 1);
                    for(int j = 0; j < dictionaries[i].numOfLanguages;j++)
                    {
                        if(j == dictionaries[i].numOfLanguages - 1)
                            printf("%s\n", dictionaries[i].languages[j]);
                        else
                            printf("%s,",dictionaries[i].languages[j]);
                    }
                }
                ///check user input
                while(input < 1 || input > numOfDictionaries)
                {
                    scanf("%d", &input);
                    if(input < 1 || input > numOfDictionaries)
                        printf("Wrong option, try again:\n");
                }
                printf("Enter a word in ");
                for(int i = 0; i < dictionaries[input - 1].numOfLanguages;i++) {
                    if (i == dictionaries[input - 1].numOfLanguages - 1)
                        printf("%s:\n", dictionaries[input - 1].languages[i]);
                    else
                        printf("%s,", dictionaries[input - 1].languages[i]);
                }
                ///input from user to an string array
                insertToEnd(&(dictionaries[input - 1].wordList), &flagOfMalloc, dictionaries[input - 1].numOfLanguages);
                if(flagOfMalloc == 1)
                {
                    printf("The addition of the word has failed!\n");
                    break;
                }
                else
                {
                    printf("The word has been added successfully!\n");
                    break;
                }

            }
                ///deleting an existing word
            case DELETE_WORD:
            {
                ///when there is no dictionary the option not available
                if(numOfDictionaries == 0)
                {
                    printf("This option is not available right now, try again:\n");
                    flagMenu = 0;
                    break;
                }
                flagMenu = 1;
                int input = 0, flagOfMalloc = 0, WrongInput = 0;
                char *str = NULL, YesOrNo;
                printf("Choose a dictionary:\n");
                for(int i = 0; i < numOfDictionaries; i++)
                {
                    printf("%d. ", i + 1);
                    for(int j = 0; j < dictionaries[i].numOfLanguages;j++)
                    {
                        if(j == dictionaries[i].numOfLanguages - 1)
                            printf("%s\n", dictionaries[i].languages[j]);
                        else
                            printf("%s,",dictionaries[i].languages[j]);
                    }
                }
                ///check user input
                while(input < 1 || input > numOfDictionaries)
                {
                    scanf("%d", &input);
                    if(input < 1 || input > numOfDictionaries)
                        printf("Wrong option, try again:\n");
                }
                printf("Enter a word in %s:\n", dictionaries[input - 1].languages[FIRST]);
                str = endlessINPUT(&flagOfMalloc);
                if(str == NULL)
                {
                    printf("The deletion of the word has failed!");
                    break;
                }
                printf("Are you sure? (y/n)\n");
                scanf(" %c", &YesOrNo);
                if(YesOrNo == YES)
                {
                    delete_Word(&(dictionaries[input - 1].wordList),str,&WrongInput,dictionaries[input - 1].numOfLanguages);
                    if(WrongInput == 1)
                    {
                        printf("The deletion of the word has failed!\n");
                        free(str);//free the string to DELETE
                        str = NULL;
                        break;
                    }
                    else
                    {
                        printf("The word has been deleted successfully!\n");
                        free(str);//free the string to DELETE
                        str = NULL;
                        break;
                    }
                }
                else
                {
                    printf("The deletion of the word has been canceled.\n");
                    free(str);//free the string to DELETE
                    str = NULL;
                }
                break;
            }
                ///search for a word
            case SEARCH_WORD:
            {
                if(numOfDictionaries == 0)
                {
                    printf("This option is not available right now, try again:\n");
                    flagMenu = 0;
                    break;
                }
                flagMenu = 1;
                int input = 0, flagOfMalloc = 0;
                char *strToSearch = NULL;
                ///in case there are no dictionary then this option is not available
                printf("Choose a dictionary:\n");
                for(int i = 0; i < numOfDictionaries; i++)
                {
                    printf("%d. ", i + 1);
                    for(int j = 0; j < dictionaries[i].numOfLanguages;j++)
                    {
                        if(j == dictionaries[i].numOfLanguages - 1)
                            printf("%s\n", dictionaries[i].languages[j]);
                        else
                            printf("%s,",dictionaries[i].languages[j]);
                    }
                }
                ///check user input
                while(input < 1 || input > numOfDictionaries)
                {
                    scanf("%d", &input);
                    if(input < 1 || input > numOfDictionaries)
                        printf("Wrong option, try again:\n");
                }
                printf("Enter a word in %s:\n", dictionaries[input - 1].languages[FIRST]);
                strToSearch = endlessINPUT(&flagOfMalloc);
                if(strToSearch == NULL)
                {
                    printf("The search has failed successfully!\n");
                    break;
                }
                if(dictionaries[input - 1].wordList == NULL)
                {
                    printf("There are no translations for \"%s\" in this dictionary.\n", strToSearch);
                }
                else
                    search_Word(strToSearch, &dictionaries[input - 1]);
                free(strToSearch);//free the string to DELETE
                break;
            }
                ///deleting a dictionary
            case DELETE_DICTIONARY:
            {
                if(numOfDictionaries == 0)
                {
                    printf("This option is not available right now, try again:\n");
                    flagMenu = 0;
                    break;
                }
                flagMenu = 1;
                int input = 0, flagOfMalloc = 0, NumberOfLanguages;
                char YesOrNo, **stringArrayToFree;
                Word* WordListToFree;
                Dictionary tempDictionary, *newDictionary;
                printf("Choose a dictionary:\n");
                for(int i = 0; i < numOfDictionaries; i++)
                {
                    printf("%d. ", i + 1);
                    for(int j = 0; j < dictionaries[i].numOfLanguages;j++)
                    {
                        if(j == dictionaries[i].numOfLanguages - 1)
                            printf("%s\n", dictionaries[i].languages[j]);
                        else
                            printf("%s,",dictionaries[i].languages[j]);
                    }
                }
                ///check user input
                while(input < 1 || input > numOfDictionaries)
                {
                    scanf("%d", &input);
                    if(input < 1 || input > numOfDictionaries)
                        printf("Wrong option, try again:\n");
                }
                printf("Are you sure? (y/n)\n");
                scanf(" %c", &YesOrNo);
                if(YesOrNo == YES)
                {
                    if(numOfDictionaries > 1)
                    {
                        for(int i = input - 1; i < numOfDictionaries - 1;i++)
                        {
                            tempDictionary.languages = dictionaries[i].languages;
                            tempDictionary.wordList = dictionaries[i].wordList;
                            tempDictionary.numOfLanguages = dictionaries[i].numOfLanguages;
                            swapDictionary(&dictionaries[i], &dictionaries[i + 1]);
                            //for the last one we need one more switch
                            dictionaries[i + 1].languages = tempDictionary.languages;
                            dictionaries[i + 1].numOfLanguages = tempDictionary.numOfLanguages;
                            dictionaries[i + 1].wordList = tempDictionary.wordList;
                        }
                        ///save the memory address that need to be free
                        stringArrayToFree = dictionaries[numOfDictionaries - 1].languages;
                        WordListToFree = dictionaries[numOfDictionaries - 1].wordList;
                        NumberOfLanguages = dictionaries[numOfDictionaries - 1].numOfLanguages;
                        newDictionary = dictionaries; // hold the address while realloc
                        dictionaries = (Dictionary*)realloc(dictionaries, sizeof(Dictionary)*(numOfDictionaries - 1));
                        if(dictionaries == NULL)
                        {
                            flagOfMalloc = 1;
                            dictionaries = newDictionary;
                        }
                        else
                        {
                            //we need to free the addresses of the dictonary that we want to delete
                            for(int i = NumberOfLanguages - 1; i >= 0;i--)
                            {
                                free(stringArrayToFree[i]);
                                stringArrayToFree[i] = NULL;
                            }
                            free(stringArrayToFree);
                            Word* iterator2 = NULL ,*iterator = WordListToFree;
                            while(iterator != NULL)
                            {
                                for(int i = NumberOfLanguages - 1; i >= 0; i--)
                                {
                                    free(iterator->translations[i]);
                                    iterator->translations[i] = NULL;
                                }
                                free(iterator->translations);
                                iterator2 = iterator;
                                iterator = iterator->next;
                                free(iterator2);
                                iterator2 = NULL;
                            }
                        }
                    }
                    else // numOfDictionary == 1
                    {
                        free_Dictionary(&dictionaries[FIRST], dictionaries[FIRST].numOfLanguages);
                    }
                    if(flagOfMalloc == 0)
                    {
                        printf("The dictionary has been deleted successfully!\n");
                        numOfDictionaries--;
                        break;
                    }
                }
                else if(YesOrNo != YES)
                {
                    printf("The deletion of the dictionary has been canceled.\n");
                    break;
                }
                //need to reverse the Dictionary array order if the realloc has failed
                for(int i = numOfDictionaries - 1; i > input - 1;i--)
                {
                    tempDictionary.languages = dictionaries[i - 1].languages;
                    tempDictionary.wordList = dictionaries[i - 1].wordList;
                    tempDictionary.numOfLanguages = dictionaries[i - 1].numOfLanguages;
                    swapDictionary(&dictionaries[i - 1], &dictionaries[i]);
                    dictionaries[i].languages = tempDictionary.languages;
                    dictionaries[i].numOfLanguages = tempDictionary.numOfLanguages;
                    dictionaries[i].wordList = tempDictionary.wordList;
                    //for the last one we need one more switch
                }
                printf("The deletion of the dictionary has failed!\n");
                break;
            }
                ///exit program
            case EXIT:
            {
                int size = numOfDictionaries - 1;
                ///in case one of the dictionary failed to realloc its size then there is one extra block to be free
                if(isMemoryDictionaryExtra == 1)
                {
                    size++;
                }
                for(int i = size; i >= 0 ;i--)
                {
                    free_Dictionary(&dictionaries[i], dictionaries[i].numOfLanguages);
                }
                free(dictionaries);
                printf("Bye!\n");
                break;
            }
            default:
            {
                printf("Wrong option, try again:\n");
                break;
            }
        }
    }
}

/******************
* Function Name: printMenu
* Input: NONE
* Output: VOID
* Function Operation: print the menu
******************/
void printMenu()
{
    printf("Welcome to the dictionaries manager!\n"
           "Choose an option:\n"
           "1. Create a new dictionary.\n"
           "2. Add a word to a dictionary.\n"
           "3. Delete a word from a dictionary.\n"
           "4. Find a word in a dictionary.\n"
           "5. Delete a dictionary.\n"
           "6. Exit.\n");
}

/******************
* Function Name:endlessINPUT
* Input:int *flagOfMalloc
* Output: char*
* Function Operation: return an address of a dynamic allocated memory of string array from user.
******************/
char* endlessINPUT(int *flagOfMalloc)
{
    char input = SPACE_CHAR, *string, *temp;
    *flagOfMalloc = 0;
    string = (char*)malloc(1*sizeof(char));
    if(string == NULL)
    {
        *flagOfMalloc = 1;
    }
    int i = 0;
    scanf(SPACE_STRING); // ignore the blank chars
    while(input != '\n' && *flagOfMalloc == 0)
    {
        scanf("%c", &input);
        if(input != ENTER)
        {
            temp = string;
            string = (char*)realloc(string, sizeof(char)*(i + 1));
            if(string == NULL)
            {
                *flagOfMalloc = 1;
                free(temp);
                return NULL;
            }
            string[i] = input;
            i++;
        }
    }
    temp = string;
    string = (char*)realloc(string, sizeof(char)*(i + 1));
    if(string == NULL)
    {
        *flagOfMalloc = 1;
        free(temp);
        return NULL;
    }
    string[i] = END_STRING_CHAR;
    return string;
}

/******************
* Function Name:stringToArray
* Input:int* numOfLanguages, int* flagOfMalloc
* Output:return char**
* Function Operation:This function gets an input from the user in the form of ___,___,_____, and stored it in
and new dynamic array that allocated in the function.
******************/
char** stringToArray(int* numOfLanguages, int* flagOfMalloc)
{
    *flagOfMalloc = 0; //indicate if malloc/realloc failed.
    *numOfLanguages = 0; //counter for the words.
    ///variable deceleration
    char input = SPACE_CHAR, **array, **tempArr = NULL, *tempStr = NULL, pre = SPACE_CHAR;
    int count = 0, flag = 0;
    ///malloc for the first array
    array = (char**)malloc(sizeof(char*)*(*numOfLanguages+1));
    if(array == NULL)
    {
        *flagOfMalloc = 1;
        return NULL;
    }
    scanf(SPACE_STRING);//ignores the blank chars
    //while loop until the user input is enter
    while(input != ENTER && *flagOfMalloc != 1)
    {
        //while loop that stops until the input is , or enter
        while(input != COMMA && input != ENTER)
        {
            scanf("%c", &input);
            if(input != COMMA && input != ENTER)
            {
                ///First time to initialize the memory
                if(count == 0)
                {
                    array[*numOfLanguages] = (char*)malloc(sizeof(char)*(count + 2));
                    if(array[*numOfLanguages] == NULL)//if malloc failed
                    {
                        for(int i = *numOfLanguages - 1; i >= 0 ;i--)
                        {
                            free(array[i]);
                            array[i] = NULL;
                        }
                        free(array);
                        *flagOfMalloc = 1;
                        return NULL;
                    }
                }
                    ///When it is not the first time of the initialize
                else
                {
                    tempStr = array[*numOfLanguages];//temp string to hold the array[*numOfLanguages] address
                    array[*numOfLanguages] = (char*)realloc(array[*numOfLanguages], sizeof(char)*(count+2));
                    if(array[*numOfLanguages] == NULL)
                    {
                        free(tempStr);//free the old dynamic memory.
                        tempStr = NULL;
                        for(int i = *numOfLanguages - 1; i >= 0 ;i--)
                        {
                            free(array[i]);
                            array[i] = NULL;
                        }
                        array = NULL;
                        *flagOfMalloc = 1;//realloc failed
                        return NULL;
                    }
                }
                pre = input;
                array[*numOfLanguages][count] = input;
                count++;//char word counter
            }
            else if((input == COMMA && pre == COMMA) || (input == ENTER && pre == COMMA))
            {
                array[*numOfLanguages] = (char*)malloc(sizeof(char)*(count + 1));
                if(array[*numOfLanguages] == NULL)//if malloc failed
                {
                    for(int i = *numOfLanguages - 1; i >= 0 ;i--)
                    {
                        free(array[i]);
                        array[i] = NULL;
                    }
                    free(array);
                    *flagOfMalloc = 1;
                    return NULL;
                }
                pre = SPACE_CHAR;
                flag = 1;
                break;
            }
        }
        array[*numOfLanguages][count] = END_STRING_CHAR;
        (*numOfLanguages)++;//next word
        if(input != ENTER)
        {
            tempArr = array;//temp arr to hold to address
            array = (char**)realloc(array, sizeof(char*)*(*numOfLanguages+1));
            if(array == NULL)
            {
                for(int i = 0; i < *numOfLanguages;i++)
                {
                    free(tempArr[i]);
                    tempArr[i] = NULL;
                }
                free(tempArr);//free the old address
                tempArr = NULL;
                *flagOfMalloc = 1;//realloc failed
                return NULL;
            }
            if(flag == 0)
                pre = input;
            input = SPACE_CHAR;// change the input because the input before is ,
            flag = 0;
        }
        count = 0;//new word, new counter
    }
    return array;// return the address
}

/******************
* Function Name:insertToEnd
* Input:Word** root, int *flagOfMalloc
* Output: VOID
* Function Operation: Take an string input from user and insert it in the last place of the linked list of the
translation of the dictionary
******************/
void insertToEnd(Word** root, int *flagOfMalloc, int numOflanguages)
{
    *flagOfMalloc = 0;
    char **array; // temp array
    int numOfTranslation; // number of translation *just for the function below*
    array = stringToArray(&numOfTranslation, flagOfMalloc);
    if(numOfTranslation != numOflanguages || array == NULL)
    {
        ///wrong input/malloc failed
        for(int i = numOfTranslation - 1;i >= 0; i--)
        {
            free(array[i]);
            array[i] = NULL;
        }
        free(array);
        *flagOfMalloc = 1;
        return;
    }
    ///check if the word excited already
    Word* iterator2 = *root;
    while(iterator2 != NULL)
    {
        ///means that the word is already exist
        if(strcmp(array[FIRST], iterator2->translations[FIRST]) == 0)
        {
            for(int i = numOfTranslation - 1;i >= 0; i--)
            {
                free(array[i]);
                array[i] = NULL;
            }
            free(array);
            *flagOfMalloc = 1;
            return;
        }
        iterator2 = iterator2->next;
    }
    Word *newTranslation = (Word*)malloc(sizeof(Word));
    if (newTranslation == NULL)
    {
        for(int i = numOfTranslation - 1;i >= 0; i--)
        {
            free(array[i]);
            array[i] = NULL;
        }
        *flagOfMalloc = 1; // malloc failed
        return;
    }
    newTranslation->translations = array; //insert the temp array in the linked list
    newTranslation->next = NULL; // last linked list struct will point to NULL
    if(*root == NULL) // when the linked list is empty
    {
        *root = newTranslation;
        return;
    }
    Word* iterator = *root;
    ///going through out the linked list till the last place
    while (iterator->next != NULL)
    {
        iterator = iterator->next;
    }
    iterator->next = newTranslation;/// inserting to the linked list
}

/******************
* Function Name:delete_Word
* Input:Word **root, char *strToDelete, int *WrongInput, int sizeOfArray
* Output: VOID
* Function Operation: take a string and a dictionary and delete the word from the dictionary.
******************/
void delete_Word(Word **root, char *strToDelete, int *WrongInput, int sizeOfArray)
{
    ///variable declaration
    Word* iterator = *root, *blockbefore;
    blockbefore = iterator;
    int count = 0;
    while(iterator != NULL)
    {
        //compare the strToDelete to the first word of the array translation
        if(strcmp(iterator->translations[FIRST], strToDelete) != 0)
        {
            blockbefore = iterator;
            iterator = iterator->next;
            count++;
        }
        else
        {
            ///breaks out from the while to start the deletion
            break;
        }
    }
    if(iterator == NULL) // the input was never found in the dictionary words
    {
        ///incorrect input
        *WrongInput = 1; //indicate if the input was incorrect
        return;
    }
    if(count > 0) //if the linked list is not empty
    {
        blockbefore->next = iterator->next;
    }
    else if(count == 0 && iterator->next != NULL)
    {
        //if it is the first word in the linked list
        *root = iterator->next;
    }
    else
        *root = NULL; // the linked list is empty
    for(int i = sizeOfArray - 1; i >= 0 ;i--)
    {
        free(iterator->translations[i]);
        iterator->translations[i] = NULL;
    }
    free(iterator->translations);
    iterator->translations = NULL;
    free(iterator);
    iterator = NULL;
}

/******************
* Function Name:search_Word
* Input:char* strToSearch, Dictionary *dictionary
* Output: VOID
* Function Operation: search throughout a given dictionary for a word from the user
******************/
void search_Word(char* strToSearch, Dictionary *dictionary)
{
    ///variable declaration
    Word* iterator;
    iterator = dictionary->wordList;
    ///going through the linked list in search for the user word
    while(strcmp(iterator->translations[FIRST],strToSearch) != 0)
    {
        iterator = iterator->next;
        if(iterator == NULL)//if the iterator went throughout the whole linked list and didnt found
        {
            printf("There are no translations for \"%s\" in this dictionary.\n", strToSearch);
            return;
        }
    }
    printf("The translations are:\n");
    ///for loop for the print format of the translation all the cases:
    for(int i = 1; i < dictionary->numOfLanguages;i++)
    {
        //if there is no translation in this languages
        if(strcmp(iterator->translations[i], END_STRING) == 0 || strcmp(iterator->translations[i], SPACE_STRING) == 0)
        {
            continue;
        }
        if(i == dictionary->numOfLanguages - 1)
        {
            printf("%s: %s\n",dictionary->languages[i], iterator->translations[i]);
        }
        else if(i < dictionary->numOfLanguages - 1)
        {
            if((i + 1 == dictionary->numOfLanguages - 1) && (strcmp(iterator->translations[i + 1], END_STRING) == 0 ||
                                                             strcmp(iterator->translations[i + 1], SPACE_STRING) == 0))
            {
                printf("%s: %s\n",dictionary->languages[i], iterator->translations[i]);
                break;
            }
            else if(strcmp(iterator->translations[i + 1], END_STRING) != 0 || strcmp(iterator->translations[i + 1], SPACE_STRING) != 0)
            {
                printf("%s: %s, ",dictionary->languages[i], iterator->translations[i]);
            }
            else
            {
                printf("%s: %s\n",dictionary->languages[i], iterator->translations[i]);
            }
        }
    }
}

/******************
* Function Name:free_Dictionary
* Input:Dictionary *dictionary
* Output: VOID
* Function Operation: free the memory of an entire Dictionary
******************/
void free_Dictionary(Dictionary *dictionary, int sizeOfLanguages)
{
    Word* iterator = dictionary->wordList;
    Word* iterator2 = NULL;
    ///run through the linked list
    while(iterator != NULL)
    {
        for(int i = sizeOfLanguages - 1; i >= 0;i--)
        {
            if(iterator->translations[i] != NULL)
            {
                free(iterator->translations[i]);//freeing each string in the translation
                iterator->translations[i] = NULL;
            }
        }
        iterator2 = iterator->next;
        free(iterator->translations);//free the entire word in the linked list
        free(iterator);
        iterator = iterator2;
    }
    for(int i = sizeOfLanguages - 1; i >= 0;i--)
    {
        free(dictionary->languages[i]);
        dictionary->languages[i] = NULL;
    }
    free(dictionary->languages);
    dictionary->languages = NULL;
}

/******************
* Function Name:swapDictionary
* Input:Dictionary Dictionary* First, Dictionary* Second
* Output: VOID
* Function Operation: swap the places of two dictionary
******************/
void swapDictionary(Dictionary* First, Dictionary* Second)
{
    Dictionary *temp;
    temp = First;
    *First = *Second;
    *Second = *temp;
}


