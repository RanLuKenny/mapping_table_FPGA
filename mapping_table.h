#pragma once

#ifndef __MAPPING_TABLE__
#define __MAPPING_TABLE__

#include <stdlib.h>
#include <stdio.h>

/*KeyValue(X-bits), here using 32 bits int*/
/*ValurType here is int*/
typedef int KeyType, ValueType;

/*Table count (Y-bits), here using 32 bits unsigned int*/
static const unsigned int TableSize = 1024;

//using this structure to express the status for each element
typedef enum status
{
    Not_exist = 0,    //0 Empty: deleted or just initialized
    Valid,    //1 valid //assigned with element, can be read
}status;

//single node for key-value with status
typedef struct Elem
{
    KeyType key;
    ValueType value;
    status stat;
}Elem;

//table's basic structure: array + count
typedef struct MappingTable
{
    Elem element[TableSize]; //storage array
    unsigned int count; //the number of elements in the table
}MappingTable;

//print function for simulation only
void Print(MappingTable* ht);

//API: initialize the table
void MappingTableInit(MappingTable* ht);

//convert function: X-bits to Y-bits
unsigned int ConvertFunction(KeyType key);

//API: insert/write key-value into the table
int MappingTableInsert(MappingTable* ht, KeyType key, ValueType value);

//API: look-up if a key is inside the table
short MappingTableLookup(MappingTable* ht, KeyType key);

//API: look-up if a key is inside the table and return its value
short MappingTableRead(MappingTable* ht, KeyType key, ValueType* value);

//API: remove one element from the talbe by giving the key
void TableDelete(MappingTable* ht, KeyType key);

//API: judge if the table is empty
short TableEmpty(MappingTable* ht);


//top level function
void table_top1( KeyType key,
				ValueType input_value,
				ValueType output_value,
				short operate,
				short *flag,
				short *not_found);

#endif
