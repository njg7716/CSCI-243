//// \file table.h
/// \brief A generic hash table data structure.
/// Author: Sean Strout (RIT CS)

#include <stdbool.h>    // bool
#include <stddef.h>     // size_t
#include <stdio.h>
#include <string.h>

/// Initial capacity of table upon creation
#define INITIAL_CAPACITY 16

/// The load at which the table will rehash
#define LOAD_THRESHOLD 0.75

/// The table size will double upon each rehash
#define RESIZE_FACTOR 2

#define MAX_NAME_LEN	7

typedef enum type_en {
    Integer, Float, Unknown
} Type;

typedef union val_un {
    int iVal;       
    float fVal;  
} Value;

typedef struct symbol_st {
    char name[MAX_NAME_LEN+1];	// allow a trailing NUL
    Value value;		
    Type type;
} Symbol;

/// The hash table
typedef struct Table_t {
    Symbol** table;    ///< table of Symbol pointers
    size_t size;      ///< number of entries in table
    size_t capacity;  ///< total capacity of table
    size_t rehashes;        ///< number of rehashes throughout life of hash table
} Table;

/// Create a new hash table.
/// @param hash The key's hash function
/// @param equals The key's equal function for comparison
/// @param print A print function for the key/value, used for dump debugging
/// @exception Assert fails if can't allocate space
/// @return A newly created table
Table* create();

/// Destroy a table
/// @param t The table to destroy
void destroy(Table* t);// \file table.h

// returns the value associated with the key
Symbol* get(Table* t, char* name);

// returns a list of all the keys in the table
void** keys(Table* t);

// Puts something new in the table
void put(Table* t, char* name, Value v, Type type);

//Author: Nicholas Graca
// Date: 12/3/17
Table* readSymbolTable(FILE* fp, Table* t);

void dump(char* name, Value value, Type type);

void dumpSymbolTable(Table* t);

bool has(Table* t, char* name);
