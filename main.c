//---------------------------------------------------------------------------------------------------------------------
// Electronic Shopping Process 
//
// Used functions and memory allocations as needed, fixed my 
// knowledge in memory allocations
// 
//---------------------------------------------------------------------------------------------------------------------
//

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define BUFFERSIZE 5

struct _Item_
{
  char* item_name_;
  char* quantity_;
};

typedef struct _Item_ Item;

bool isMemoryAllocated(void* pointer);

int inputShoppingListName(char** list_name);

int inputShoppingList(Item** shopping_list, char* shopping_list_name, size_t* amount);

void initializeList(Item* shopping_list, size_t item_count, size_t old_count);

bool isQuit(char* checked);

bool isDone(char* checked);

bool isNameEmpty(size_t length);

int inputItem(Item* shopping_list, char* shopping_list_name, size_t amount);

int inputQuantity(Item* shopping_list, char* shopping_list_name, size_t item_count);

bool isInteger(char* item_quantity);

bool isPositive(char* item_quantity);

void removePlus(char* item_quantity);

void sortShoppingList(Item* shopping_list, size_t element_count);

void printShoppingList(char* shopping_list_name, Item* shopping_list, size_t element_count);

void freeAllItems(Item* shopping_list, size_t amount);

//------------------------------------------------------------------------------
///
/// The main program.
/// Main is connecting the
/// outside functions that do most of the work
/// In main we also declare returns dependant of the type of a result
/// 
///
///
/// @param argc not used
/// @param argv not used
///
/// @return 0 - quit or done; 1 - Memory Allocation Failed
//
int main(void)
{
  printf("\nWelcome to the\nElectronic Shopping Process!\n\n");

  char* shopping_list_name = NULL;
  int shopping_list_name_check = 0;
  shopping_list_name_check = inputShoppingListName(&shopping_list_name);

  if (shopping_list_name_check == 1)
  {
    return 1;
  }
  else if (shopping_list_name_check == -1)
  {
    return 0;
  }

  Item* shopping_list = NULL;
  size_t curr_index = 0;
  int shopping_list_check = 0;
  shopping_list_check = inputShoppingList(&shopping_list, shopping_list_name, &curr_index);

  if (shopping_list_check == 1)
  {
    return 1;
  }
  else if (shopping_list_check == -1)
  {
    return 0;
  }

  sortShoppingList(shopping_list, curr_index);

  printShoppingList(shopping_list_name ,shopping_list, curr_index);

  freeAllItems(shopping_list, curr_index + 1);

  free(shopping_list_name);
  shopping_list_name = NULL;

  return 0;
}


//---------------------------------------------------------------------------------------------------------------------
///
/// Takes an allocated pointer and checks if the Memory Allocation Failed
///
/// @param pointer any pointer that points to an allocation
/// @return true - memory allocated; false - memory NOT allocated
bool isMemoryAllocated(void* pointer)
{
  if (pointer == NULL)
  {
    return false;
  }
  return true;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Takes input for shopping list name
///
/// @param list_name pointer which will point to the shopping list name 
///                  on heap
/// @return 1 - Memory Allocation Failed; 0 - successful input
int inputShoppingListName(char** list_name)
{
  size_t length = 5;
  char* shopping_list_name = NULL;
  shopping_list_name = (char*) calloc(length, sizeof(char));
  if (!isMemoryAllocated(shopping_list_name))
  {
    printf("[ERROR] Memory allocation failed!\n\n");
    free(shopping_list_name);
    shopping_list_name = NULL;
    return 1;
  }

  while (true)
  {
    printf("What do you want to name your shopping list?\n > ");

    char input = 0;
    size_t curr_index = 0;

    while ((input = getchar()) != '\n' && input != EOF)
    {
      if (curr_index >= length - 1)
      {
        length += BUFFERSIZE;
        char* temp = (char*) realloc(shopping_list_name, length);
        if (!isMemoryAllocated(temp))
        {
          free(shopping_list_name);
          shopping_list_name = NULL;
          printf("[ERROR] Memory allocation failed!\n\n");
          return 1;
        }
        shopping_list_name = temp;
        temp = NULL;
      }
      shopping_list_name[curr_index++] = input;
    }
    shopping_list_name[curr_index] = '\0';

    if (isNameEmpty(curr_index))
    {
      printf("[ERROR] The name cannot be empty!\n\n");
      continue;
    }

    if (isQuit(shopping_list_name))
    {
      free(shopping_list_name);
      shopping_list_name = NULL;
      return -1;
    }

    *list_name = shopping_list_name;
    shopping_list_name = NULL;
    break;
  }
  return 0;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Takes input for item_name and for item_quantity and stores it in
/// shopping list struct
///
/// @param shopping_list struct array with items
/// @param shopping_list_name shopping list name
/// @param amount pointer that passes a value (number of items in the list) to main
/// @return 1 - Memory Allocation Failed; 0 - successful input; 
///         -1 - quit as input; 2 - done as input
int inputShoppingList(Item** shopping_list, char* shopping_list_name, size_t* amount)
{
  size_t item_count = 2;
  size_t curr_index = 0;
  Item* list = (Item*) calloc(item_count, sizeof(Item));
  if (!isMemoryAllocated(list))
  {
    printf("[ERROR] Memory allocation failed!\n\n");
    freeAllItems(list, item_count);
    free(shopping_list_name);
    shopping_list_name = NULL;
    return 1;
  }
  initializeList(list, item_count, curr_index);

  while (true)
  {
    if (curr_index >= item_count - 1)
    {
      item_count = curr_index + 1;
      Item* temp = (Item*) realloc(list, item_count * sizeof(Item));
      if (!isMemoryAllocated(temp))
      {
        printf("[ERROR] Memory allocation failed!\n\n");
        freeAllItems(list, curr_index + 1);
        free(shopping_list_name);
        shopping_list_name = NULL;
        return 1;
      }
      list = temp;
      temp = NULL;
      
      initializeList(list, item_count, curr_index);
    }
    printf("\nItem %zu:\n", curr_index + 1);

    if (inputItem(list, shopping_list_name, curr_index) == 1)
    {
      return 1;
    }
    if (isQuit(list[curr_index].item_name_))
    {
      freeAllItems(list, curr_index + 1);
      free(shopping_list_name);
      shopping_list_name = NULL;
      return -1;
    }
    if (isDone(list[curr_index].item_name_))
    {
      break;
    }

    size_t quantity_error_flag = inputQuantity(list, shopping_list_name, curr_index);
    if (quantity_error_flag == 1)
    {
      return 1;
    }
    else if (quantity_error_flag == 2)
    {
      freeAllItems(list, curr_index + 1); 
      free(shopping_list_name); 
      shopping_list_name = NULL;
      return -1;
    }
    curr_index++;
  }

  if (curr_index == 0)
  {
    printf("\nShopping list: %s\n", shopping_list_name);
    freeAllItems(list, curr_index + 1);
    free(shopping_list_name);
    shopping_list_name = NULL;
    return -1;
  }

  *amount = curr_index;
  *shopping_list = list;
  list = NULL;
  return 0;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Initializing the shopping_list to NULL, from the latest unfilled 
/// element of the new reallocated array
///
/// @param shopping_list struct array with items
/// @param item_count current number of elements
/// @param old_count old number of elements
/// @return no return
void initializeList(Item* shopping_list, size_t item_count, size_t old_count)
{
  for (size_t index = old_count; index < item_count; index++) 
  {
    shopping_list[index].item_name_ = NULL;
    shopping_list[index].quantity_ = NULL;
  }
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Takes a string and compares it with "quit"
///
/// @param checked string that is checked
/// @return true - quit; false - NOT quit
bool isQuit(char* checked)
{
  if (strcmp(checked, "quit") == 0)
  {
    return true;
  }
  return false;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Takes a string and compares it with "done"
///
/// @param checked string that is checked
/// @return true - done; false - NOT done
bool isDone(char* checked)
{
  if (strcmp(checked, "done") == 0)
  {
    return true;
  }
  return false;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Checks if string is empty
///
/// @param length 
/// @return true - length is 0 / empty string; false - length NOT 0
bool isNameEmpty(size_t length)
{
  if (length == 0)
  {
    return true;
  }
  return false;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Takes input for item_name from user and stores it in a temporary pointer
/// which is, if valid, passed to the shopping_list item_name_
///
/// @param shopping_list struct array with items
/// @param shopping_list struct array with items
/// @param item_count current element in the struct shopping_list
/// @return 1 - Memory Allocation Failed; 0 - successfully inputed item_name
int inputItem(Item* shopping_list, char* shopping_list_name, size_t item_count)
{
  size_t length = 1;
  char input = 0;
  size_t curr_character = 0;
  char* item_name = (char*) calloc(length, sizeof(char));
  if (!isMemoryAllocated(item_name))
  {
    printf("[ERROR] Memory allocation failed!\n\n");
    
    free(item_name);
    item_name = NULL;
    freeAllItems(shopping_list, item_count);
    free(shopping_list_name);
    shopping_list_name = NULL;
    return 1;
  }

  while (true)
  {
    printf("What do you want to add?\n > ");

    while ((input = getchar()) != '\n')
    {
      if (curr_character >= length - 1)
      {
        length += BUFFERSIZE; 
        char* item_name_temp = (char*)realloc(item_name, length);
        if (!isMemoryAllocated(item_name_temp))
        {
          printf("[ERROR] Memory allocation failed!\n\n");
          
          free(item_name);
          item_name = NULL;
          freeAllItems(shopping_list, item_count);
          free(shopping_list_name);
          shopping_list_name = NULL;
          return 1;
        }
        item_name = item_name_temp;
        item_name_temp = NULL;
      }
      item_name[curr_character++] = input;
    }
    item_name[curr_character] = '\0';

    if (isNameEmpty(curr_character))
    {
      printf("[ERROR] Item name cannot be empty!\n\n");
      
      free(item_name);
      item_name = NULL;
      
      curr_character = 0;
      continue;
    }
    break;
  }
  shopping_list[item_count].item_name_ = item_name;
  item_name = NULL;

  return 0;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Takes input for item_quantity from user and stores it in a temporary pointer
/// which is, if valid, passed to the shopping_list item_quantity_
///
/// @param shopping_list struct array with items
/// @param shopping_list struct array with items
/// @param item_count current element in the struct shopping_list
/// @return 1 - Memory Allocation Failed; 2 - input: "quit" ; 0 - successfully inputed item_name
int inputQuantity(Item* shopping_list, char* shopping_list_name, size_t item_count)
{
  size_t length = 1;
  char input = 0;
  size_t curr_character = 0;
  char* item_quantity = (char*) calloc(length, sizeof(char));
  if (!isMemoryAllocated(item_quantity))
  {
    printf("[ERROR] Memory allocation failed!\n\n");
    
    free(item_quantity);
    item_quantity = NULL;
    freeAllItems(shopping_list, item_count);
    free(shopping_list_name);
    shopping_list_name = NULL;
    return 1;
  }

  while (true)
  {
    printf("How many pieces/grams/milliliters of this item do you want to get?\n > ");

    while ((input = getchar()) != '\n')
    {
      if (curr_character >= length - 1)
      {
        length += BUFFERSIZE; 
        char* quantity_temp = (char*)realloc(item_quantity, length);
        if (!isMemoryAllocated(quantity_temp))
        {
          printf("[ERROR] Memory allocation failed!\n\n");
          
          free(item_quantity);
          item_quantity = NULL;
          freeAllItems(shopping_list, item_count);
          free(shopping_list_name);
          shopping_list_name = NULL;
          return 1;
        }
        item_quantity = quantity_temp;
        quantity_temp = NULL;
      }
      item_quantity[curr_character++] = input;
    }
    item_quantity[curr_character] = '\0';

    if (isQuit(item_quantity))
    {
      free(item_quantity);
      item_quantity = NULL;
      return 2;
    }

    if (!isInteger(item_quantity))
    {
      curr_character = 0;
      continue;
    }
    
    if (!isPositive(item_quantity))
    {
      curr_character = 0;
      continue;
    }

    break;
  }

  removePlus(item_quantity);

  shopping_list[item_count].quantity_ = item_quantity;
  item_quantity = NULL;

  return 0;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Checks if the quantity of the item is an integer
///
/// @param item_quantity quantity of shopping_list item
/// @return true - it is an integer; false - it is NOT an integer
bool isInteger(char* item_quantity)
{
  size_t length = strlen(item_quantity);
  if (isNameEmpty(length))
  {
    printf("[ERROR] The quantity must be an integer!\n\n");
    return false;
  }

  for (size_t index = 0; index < length; index++)
  {
    if ((index == 0 && (item_quantity[index] == '+' || item_quantity[index] == '-')) && length == 1)
    {
      printf("[ERROR] The quantity must be an integer!\n\n");
      return false;
    }
    else if (index == 0 && (item_quantity[index] == '+' || item_quantity[index] == '-'))
    {
      continue;
    }

    if (!isdigit(item_quantity[index]))
    {
      printf("[ERROR] The quantity must be an integer!\n\n");
      return false;
    }

  }
  return true;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Checks if the quantity of the item is a positive number
///
/// @param item_quantity quantity of shopping_list item
/// @return true - it is positive; false - it is NOT positive
bool isPositive(char* item_quantity)
{
  bool is_number = false;
  bool is_all_zero = true;
  for (size_t index = 0; index < strlen(item_quantity); index++)
  {
    if (isdigit(item_quantity[index]))
    {
      is_number = true;
      if (item_quantity[index] != '0')
      {
        is_all_zero = false;
      }
    }

    if (item_quantity[index] == '-' && !is_number)
    {
      printf("[ERROR] The quantity must be greater than 0!\n\n");
      return false;
    }
  }

  if (is_all_zero)
  {
    printf("[ERROR] The quantity must be greater than 0!\n\n");
    return false;
  }
  return true;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// If there is a plus in front of the quantity number, remove it
///
/// @param item_quantity quantity of the item in shopping list
/// @return no return
void removePlus(char* item_quantity)
{
  if (item_quantity[0] == '+')
  {
    for (size_t index = 0; index < strlen(item_quantity); index++)
    {
      item_quantity[index] = item_quantity[index + 1];
    }
    item_quantity[strlen(item_quantity)] = '\0';
  }
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Using bubble sort to sort the shopping list by ASCII values / quantity
///
/// @param shopping_list struct array with items
/// @param element_count number of elements in the shopping list
/// @return no return
void sortShoppingList(Item* shopping_list, size_t element_count)
{
  for (size_t i = 0; i < element_count - 1; i++)
  {
    for (size_t j = 0; j < element_count - i - 1; j++)
    {
      if (strcmp(shopping_list[j].item_name_, shopping_list[j + 1].item_name_) > 0)
      {
        Item temp = shopping_list[j];
        shopping_list[j] = shopping_list[j + 1];
        shopping_list[j + 1] = temp;
      }
      else if (strcmp(shopping_list[j].item_name_, shopping_list[j + 1].item_name_) == 0)
      {
        if (atoi(shopping_list[j].quantity_) > atoi(shopping_list[j + 1].quantity_))
        {
          Item temp = shopping_list[j];
          shopping_list[j] = shopping_list[j + 1];
          shopping_list[j + 1] = temp;
        }
      }
    }
  }
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Prints the final shopping list
///
/// @param shopping_list_name name of the shopping list
/// @param shopping_list struct array with items
/// @param element_count number of elements in the shopping list
/// @return no return
void printShoppingList(char* shopping_list_name, Item* shopping_list, size_t element_count)
{
  printf("\nShopping list: %s\n", shopping_list_name);
  for (size_t index = 0; index < element_count; index++)
  {
    printf(" * %s %s\n", shopping_list[index].quantity_,
                         shopping_list[index].item_name_);
  }
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Frees all of the elements of the shopping_list and the struct itself
///
/// @param shopping_list struct array with items
/// @param item_count number of elements in the struct array
/// @return no return
void freeAllItems(Item* shopping_list, size_t item_count)
{
  for (size_t index = 0; index < item_count; index++)
  {
    free(shopping_list[index].item_name_);
    shopping_list[index].item_name_ = NULL;

    free(shopping_list[index].quantity_);
    shopping_list[index].quantity_ = NULL;
  }

  free(shopping_list);
  shopping_list = NULL;
}
