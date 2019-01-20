#include "mapping_table.h"
//#include "ap_int.h"

//this function could convert 48 bits to 32 bits
/*
 *
 *I will synthesize this function on FPGA,

 *The return value will be the physical address
 *
 */

unsigned int mapping(const unsigned long virtual_address){
	const unsigned int address_diff = XPAR_PS7_DDR_0_S_AXI_HIGHADDR - XPAR_PS7_DDR_0_S_AXI_BASEADDR;
	unsigned int temp = virtual_address % address_diff;

	return (temp - temp%4 + XPAR_PS7_DDR_0_S_AXI_BASEADDR);
	
}

// unsigned int ConvertFunction(KeyType key)
// {
//     return key % TableSize; //next time, use MMU method to do this
//     //range 0 to (TableSize - 1)
// }

////////////////////////////////////////////////////////////////////////////////////
void MappingTableInit(MappingTable* ht)
{
    ht->count = 0;
   // ht->hashfunc = ConvertFunction;
    for (unsigned int i = 0; i < TableSize; i++)
    {
        ht->element[i].key = 0;
        ht->element[i].stat = Not_exist;
        ht->element[i].value = 0;
    }
}

//insert the value
int MappingTableInsert(MappingTable* ht, KeyType key/*, ValueType value*/)
{
	ValueType value = mapping(key);
    if (ht->count >= TableSize)
        return 0;
    
    unsigned int  base = ConvertFunction(key);

    //this loop will cause unknown delay. but we know the minimum and maximum value
    //because we dont know the exact value of 'cur' every time

    //for(; base  < TableSize; base++) //??? delay is ?
    //
    for(unsigned int i = 0 ; i < TableSize; i++) //delay now fixed
	{
		if(i==base ){
			//cur = i;
			if (ht->element[i].key == key)
				return 0;

			if (ht->element[i].stat != Valid)
			{
				ht->element[i].key = key;
				ht->element[i].value = value;
				ht->element[i].stat = Valid;
				ht->count++;
				return 1;
			}
			base++;
		}
	}
}

//synthesis problem
short MappingTableRead(MappingTable* ht, KeyType key, ValueType *value)
{
	//ValueType value = mapping(key);
    unsigned int offset=ConvertFunction(key);
    if (ht->element[offset].key == key && ht->element[offset].stat==Valid)
    {
        *value = ht->element[offset].value;
        return 0; //found
    }
    else //if not found
    {
    	for(unsigned int i = 0; i < TableSize; i++)
        {

    		if(i==offset){
				if (ht->element[i].key != key)
				{
					offset++;

					if (offset >= TableSize)
						offset = 0;
				}
				else
				{
					if (ht->element[i].stat == Valid)
					{
						*value = ht->element[i].value;
						return 0;
					}
					else
						offset++;
				}
    		}
        }
        return 1;
    }
}

//look up function
short MappingTableLookup(MappingTable* ht, KeyType key)
{
	unsigned int offset=ConvertFunction(key);
	if (ht->element[offset].key == key && ht->element[offset].stat==Valid)
	{
	    return 0; //found
	}
	else //if not found, find the next following address
	{
		//the iteration could have directly started from offset or not 0;
		//but if I dont use 0, the lantency will be unknown
		//sacrifice the delay for hardware synthesis
	    for(unsigned int i = 0; i < TableSize; i++)
	    {
	    	if(i==offset){
				if (ht->element[i].key != key)
				{
					offset++;

					if (offset >= TableSize)
						offset = 0;
				}
				else
				{
					if (ht->element[i].stat == Valid)
					{
						return 0;
					}
					else
						offset++;
					}
	    		}
	        }
	        return 1;
	  }
}

/*find the base address from convert function*/
int MappingTableFindBase(MappingTable* ht, KeyType key, unsigned int *base)
{
    for (unsigned int i = 0; i < TableSize; i++)
    {

        if (ht->element[i].key == key && ht->element[i].stat == Valid)
        {
            *base = i;
            return 1; //element found
        }
    }
    return 0; //element not found
	//printf("\nnot found\n");
}

/*API: delete the element by giving key*/
void TableDelete(MappingTable* ht, KeyType key)
{
    ValueType value = 0;

    //start from 0
    unsigned int base = 0;

    //start from 0 to the end, to search the whole table
    int ret=MappingTableFindBase(ht,key,&base);

    if (ret == 0) //return 0
        return;
    else
    {
        ht->element[base].stat = Not_exist; //invalid before
        ht->count--;
    }
}

//return the value of count
//
short TableEmpty(MappingTable* ht)
{
        return (ht->count);
}

void Print(MappingTable* ht)
{
    for (int i = 0; i < TableSize; i++)
    {
    /*If you want to display the rest of the element, just comment the 'if' syntax below*/
	//but,,,,,may display huge number of elements
		if (ht->element[i].stat == Valid)
			printf("[%d]  key=0x%x  value=0x%x  stat=%d\n", i, ht->element[i].key, ht->element[i].value, ht->element[i].stat);
    }
}

//top-function

/**operate**/
	//delete---- read ---- write ---- lookup ----
	//read: 1 (binary: 0001)
	//write: 2 (binary: 0010)
	//lookup: 4 (binary: 0100)
	//delete: 8 (binary: 1000)
	//

	/**flag signal*/
	//flag = 0: wait for operation mode, nothing happens
	//flag = 1: read mode
	//flag = 2: write mode
	//flag = 3: lookup mode
	//flag = 4: delete mode

	//not found signal = 1: the key not found

void table_top1( KeyType key,
				ValueType input_value,
				ValueType output_value,
				short operate,
				short *flag, //output
				short *not_found) //output
{
	MappingTable ht;
	MappingTableInit(&ht);

	switch(operate){
		case 1: //read
			*flag = 1;
			*not_found = MappingTableRead(&ht, key, &output_value); //latency 1 here
			break;
		case 2: //insert
			*flag = 2;
			MappingTableInsert(&ht, key); //latency 2 here
			break;
		case 4: //lookup
			*flag = 3;
			*not_found = MappingTableLookup(&ht, key);
			break;
		case 8: //delete
			*flag = 4;
			TableDelete(&ht, key);
			break;
		default:
			*flag = 0;
			break;
	}
}

 
