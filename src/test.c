////////////test
#include "mapping_table.h"

#define RAM_SIZE 1024

///*
#define __TestHead__ printf("\n-----------------------%s--------------------------\n",__FUNCTION__);

void testMappingTableInsert(MappingTable* ht)
{
    __TestHead__;
    MappingTableInit(ht);
    MappingTableInsert(ht, 1);
    MappingTableInsert(ht, 2);
    MappingTableInsert(ht, 1001);
    MappingTableInsert(ht, 1002);
    MappingTableInsert(ht, 10001);
    MappingTableInsert(ht, 1002);
    Print(ht);
}

void testMappingTableFind(MappingTable* ht)
{
    __TestHead__;
    MappingTableInit(ht);
    MappingTableInsert(ht, 1);
    MappingTableInsert(ht, 2);
    MappingTableInsert(ht, 1001);
    MappingTableInsert(ht, 102);
    MappingTableInsert(ht, 10001);
    MappingTableInsert(ht, 1002);
    ValueType value = 0;
    int ret = MappingTableRead(ht, 1002, &value);
    printf("ret: expect=1 actual = 0x%x value: expect=0x%x actual=0x%x\n", ret, mapping(1002), value);
}

void testTableDelete(MappingTable* ht)
{
    __TestHead__;
    MappingTableInit(ht);
    MappingTableInsert(ht, 1);
    MappingTableInsert(ht, 2);
    MappingTableInsert(ht, 1001);
    MappingTableInsert(ht, 102);
    MappingTableInsert(ht, 10001);
    MappingTableInsert(ht, 1002);
    Print(ht);
    TableDelete(ht, 1001);
    Print(ht);
    MappingTableInsert(ht, 100001);
    Print(ht);

}

//int main()
//{
//    MappingTable ht;
//    MappingTableInit(&ht);
//    testMappingTableInsert(&ht);
//    testMappingTableFind(&ht);
//    testTableDelete(&ht);
//    printf("Happy new year!");
//}//*/
