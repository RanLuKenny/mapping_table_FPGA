# mapping_table_FPGA

File folder 'src' contains the C source code files in SDK PS and PL cosim

File folder 'DDR_mapping_Vivado_SDK' contains the compressed project files of Vivado and SDK part

The basic idea is that: 

There are two ways you could use to map the address:
    1. Put the virtual-physical address entry's pairs in the table. insert. delete. read .write. Then using the mapping function in SDK to access the DRAM's physical address by AXI CDMA module.
    
    2. If you don't want to use the tables. you can directly use the mapping() function to convert the address
