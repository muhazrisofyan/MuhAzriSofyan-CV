/*
 * (c) 2017 Achmad Firdaus Adinegoro & Fersandi Pratama -- This is free software
 * Feel free to copy and/or modify and/or distribute it, 
 * provided this notice, and the copyright notice, are preserved. 
 * REV01 Mon Oct  2 2017
 * START Mon Sep 25 2017
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define BASE_ADDRESS 0x00200000
char *load_8bytes_from_memory(char* p_a);

/* Manual implementation of single Hexadecimal char to Integer Converter 
 * source: https://stackoverflow.com/questions/28268753/convert-single-character-hex-number-to-integer-in-c
 */
int hex2int(char d) {
	if (d >= '0' && d <= '9') {
		return d - '0';
	}
	d = tolower(d);
	if (d >= 'a' && d <= 'f') {
		return (d - 'a') + 10;
	}
	return -1;
}

/*
 * Convert String to Integer
 * Constraint: cannot goes about INT_MAX since it's signed int
 */
int hexstr2int(char *hex){
	int hexLen = strlen(hex);
	int hexInt = 0, base = 1; 
	int i;
	for (i = hexLen - 1; i >= 0 ; i--){
		hexInt += hex2int(hex[i]) * (base);
		base = base << 4;
	}

	return hexInt;
}

/*
 * Convert integer to Char with hexadecimal value
 */
char int2hex(int num){
	if (num < 10){
		return num + '0';
	} else {
		return (num - 10) + 'A';
	}
}

/*
 * Convert Integer to String of Hexadecimal value
 */
void int2hexstr(int address, char *convert){
	sprintf(convert, "%08X", address);
}

/* Implement This ! 
 * This method will convert Virtual Memory Address to Physical Memory Address
 */
char *convert(char* address) {
	//Convert this annoying string to integer first
	int addressInt = hexstr2int(address);
	int vp1 = addressInt  >> 22;
	int vp2 = (addressInt & 0x003ff000) >> 12; 
	int offset = addressInt & 0xfff;
	char *phyAddr1 = malloc(sizeof(char)*8);
	char *phyAddr2 = malloc(sizeof(char)*8);
	char *frameNumber = malloc(sizeof(char)*8);
	int2hexstr(BASE_ADDRESS+4*vp1,phyAddr1);
	int pte1 = hexstr2int(load_8bytes_from_memory(phyAddr1));
	int valid = pte1 & 1;
	if(!valid)return "Address Invalid";
	int2hexstr((pte1 & 0xfffff000)+4*vp2,phyAddr2);
	int pte2 = hexstr2int(load_8bytes_from_memory(phyAddr2));
	valid = pte2 & 1;
	if(!valid) return "Address Invalid"; 
	int2hexstr((pte2 & 0xfffff000)+offset,frameNumber);
	/*
	 * TODO:
	 * Implement the Conversion based on the specification on this week instruction!
	 * HINT: 
	 * * To 32 bits of PTE use load_8bytes_from_memory method
	 * * To Convert the String to Integer use hexstr2int(yourstring), return value is the integer
	 * * To Convert the Integer to String use int2hexstr(inputint, outputstring), return value is String in Hexa
	 * *
	 * * How to cut the strings? Convert it to integer first, then use bitwise operation
	 * * * Syntax: int a = some_int_value your_bitwise_operation how_much_bit_you_want_to_shift;
	 * * * 		   << is right shift, >> is left shift.
	 * * * Example: int a = 4 << 2; will result in 16.
	 * *
	 * * Careful! String representation in C is closer to machine implementation than other language
	 * * like Python or Java. Use malloc to play it save.
	 */

	// Make sure you change this return to your String result
	return frameNumber;
}

/*
 * A Simple Representation of memory on an 2D Array
 */
char *mem[25][16] = {
					{"0E", "0F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D"}, 
					{"1E", "1F", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D"}, 
					{"40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4A", "4B", "4C", "4D", "4E", "4F"}, 
					{"40", "03", "41", "01", "30", "01", "31", "03", "00", "00", "00", "00", "00", "00", "00", "00"}, 
					{"00", "11", "22", "33", "44", "55", "66", "77", "88", "99", "AA", "BB", "CC", "DD", "EE", "FF"}, 
					{"10", "01", "11", "03", "31", "03", "13", "00", "14", "01", "15", "03", "16", "01", "17", "00"}, 
					{"10", "01", "11", "00", "12", "03", "67", "03", "11", "03", "00", "00", "00", "00", "00", "00"}, 
					{"02", "20", "03", "30", "04", "40", "05", "50", "01", "60", "03", "70", "08", "80", "09", "90"}, 
					{"10", "00", "31", "01", "10", "03", "31", "01", "12", "03", "30", "00", "10", "00", "10", "01"}, 
					{"30", "00", "31", "01", "11", "01", "33", "03", "34", "01", "35", "00", "43", "38", "32", "79"}, 
					{"50", "28", "84", "19", "71", "69", "39", "93", "75", "10", "58", "20", "97", "49", "44", "59"}, 
					{"23", "03", "20", "03", "00", "01", "62", "08", "99", "86", "28", "03", "48", "25", "34", "21"}, 
					{"00", "00", "10", "65", "00", "00", "20", "67", "00", "00", "30", "00", "00", "00", "40", "07"}, 
					{"00", "00", "50", "03", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00"}, 
					{"11", "22", "00", "05", "55", "66", "77", "88", "99", "AA", "BB", "CC", "DD", "EE", "FF", "00"}, 
					{"22", "33", "44", "55", "66", "77", "88", "99", "AA", "BB", "CC", "DD", "EE", "FF", "00", "67"}, 
					{"04", "15", "00", "00", "48", "59", "70", "7B", "8C", "9D", "AE", "BF", "D0", "E1", "F2", "03"}, 
					{"10", "15", "00", "67", "10", "15", "10", "67", "10", "15", "20", "67", "10", "15", "30", "67"}, 
					{"00", "00", "00", "00", "00", "00", "00", "65", "00", "00", "10", "67", "00", "00", "00", "00"}, 
					{"00", "00", "20", "67", "00", "00", "30", "67", "00", "00", "40", "65", "00", "00", "50", "07"}, 
					{"00", "00", "00", "00", "00", "00", "00", "00", "10", "00", "00", "67", "00", "10", "30", "65"}, 
					{"00", "10", "00", "07", "00", "10", "10", "07", "00", "10", "20", "07", "00", "10", "30", "07"}, 
					{"00", "10", "40", "07", "00", "10", "50", "07", "00", "10", "60", "07", "00", "10", "70", "07"}, 
					{"00", "10", "00", "07", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00", "00"}, 
					{"00", "00", "00", "00", "00", "00", "00", "00", "00", "1F", "E0", "07", "00", "1F", "F0", "07"}
					};

/* Uh.. so to map memory representation to it's physical memory, I used another 2D array
 * Since there are no Maps in C (and too lazy to make struct yada yada)
 */					
char *mem_map[25][2] = {{"0000000","0"}, {"0000001","1"}, {"0000101","2"}, {"0000102","3"}, {"0000103","4"},
						{"0000104","5"}, {"0000203","6"}, {"0000204","7"}, {"0000205","8"}, {"0000400","9"},
						{"0000401","10"}, {"0000402","11"}, {"0010000","12"}, {"0010001","13"}, {"0010300","14"}, 
						{"0010301","15"}, {"001FE00","16"}, {"001FE01","17"}, {"001FF00","18"}, {"001FF01","19"}, 
						{"001FFFF","20"}, {"0020000","21"}, {"0020001","22"}, {"0020002","23"}, {"00200FF","24"}};

/*
 * Retrieve one slot (2 Bytes) from Memory (Mem Array above) given the Physical Address
 * Source of the table address can be requested to Mr. Anwar
 * Ugh.. exhaustive way, contact me if you think any better algorithm
 *
 * Arguments:
 * char* p_a = The Physical Memory Address that we wanted to load
 *
 * Return Value:
 * One slot (2 Bytes) of Memory from arguments Address
 */
char *load_from_memory(char* p_a) {
	char *answer;
	
	/* Get the first 7 Digits to be compared */
	char first_7_digit[8];
	strncpy(first_7_digit, p_a, 7);
	first_7_digit[8] = '\0';
	
	int i, found, row, col, d;
	found = 0;
	for(i = 0; i < 25 && found == 0; i++) {
		if(strcmp(first_7_digit,mem_map[i][0]) == 0) {
			row = atoi(mem_map[i][1]);
			
			/* Convert the last char to integer */
			col = hex2int(p_a[7]);
			
			/* Retrieve from memory */
			answer = mem[row][col];
			found = 1;
		}
	}
	
	if(found) {
		return answer;
	}
	
	return "cannot be Determined, lookup error!";
	// example of expected output: "1F";
}

/*
 * Retrieve four slot (8 Bytes) from Memory (Mem Array above) given the Physical Address
 * Source of the table address can be requested to Mr. Anwar
 * Ugh.. exhaustive way, contact me if you think any better algorithm
 *
 * Arguments:
 * char* p_a = The Physical Memory Address that we wanted to load
 *
 * Return Value:
 * One slot (2 Bytes) of Memory from arguments Address
 */
char *load_8bytes_from_memory(char* p_a) {
	char *answer;
	char *memory = malloc(sizeof(char) * 9);	
	/* Get the first 7 Digits to be compared */
	char temp[9];
	strncpy(temp, p_a, 8);
	temp[8]='\0';
		
	int i;

	for(i = 0; i < 4; i++) {
		answer = load_from_memory(temp);
		strncpy(memory + sizeof(char) * (i*2), answer, 2);
		int2hexstr(hexstr2int(temp) + 1, temp);
	}
	memory[8] = '\0';
	return memory;
}

/*
 * Main Method
 */
void main(int argc, char **argv) {
	/* To read the Instruction, Only receives Convert and Load */
	char *instruction = argv[1];
	
	/* Virtual Memory that wanted to be Accessed/Converted 
	 */
	char *vm_address = argv[2]; //to read the test

	
	/* Check whether the Instruction Argument is Filled*/
	if(instruction == 0) {
		printf("-------------------\n");
		printf("INVALID SYNTAX!\n");
		printf("Syntax: ./vm-to-memory instruction vm_address\n");
		printf("Cancelling Code...\n");
		printf("-------------------\n");
		return;
	}
	
	/* Check whether the VM Address Argument is Filled*/
	if(vm_address == 0 && strcmp("Help",instruction) != 0) {
		printf("-------------------\n");
		printf("INVALID SYNTAX!\n");
		printf("Syntax: ./vm-to-memory instruction vm_address\n");
		printf("Cancelling Code...\n");
		printf("-------------------\n");
		return;
	}
	
	/* Check the Instructions */
	if(strcmp("Load",instruction) == 0) {
		
		/* Print the Necessities before doing the job */
		printf("-------------------\n");
		printf("Your Query: %s %s\n", instruction, vm_address);
		printf("Loading Address %s\n",vm_address);
		
		/* Converting the Virtual Memory Address to Physical Memory Address */
		char *physical_address = convert(vm_address);

		printf("VM Address %s has a Physical Memory Address of %s\n", vm_address, physical_address);
		
		/* Accessing the Physical Memory Address */
		char *results = load_from_memory(physical_address);
		
		printf("Content inside Physical Address %s is \'%s\'\n", physical_address, results);
		printf("-------------------\n");
	
	} else if (strcmp("Convert",instruction) == 0) {
	
		/* Print the Necessities before doing the job */
		printf("-------------------\n");
		printf("Your Query: %s %s\n", instruction, vm_address);
		printf("Converting Virtual Memory of %s to Physical Memory\n", vm_address);
		
		/* Converting the Virtual Memory Address to Physical Memory Address */
		char *results = convert(vm_address);
		
		printf("VM Address %s has a Physical Memory Address of %s\n", vm_address, results);
		printf("-------------------\n");
	
	} else if (strcmp("Help",instruction) == 0) {
		
		/* Print the Help Menu */
		printf("-------------------\n");
		printf("HELP MENU\n");
		printf("Syntax: ./vm-to-memory instruction vm_address\n");
		printf("Instruction: Load or Convert (Case Sensitive)\n");
		printf("             Load: Load a value of the given Virtual Memory Address\n");
		printf("             Convert: Convert a Virtual Memory Address value\n");
		printf("VM_address : Your query Virtual Memory address\n");
		printf("             All Address are assumed to be a Hexadecimal value, so YOU DON'T NEED 0x\n");
		printf("-------------------\n");
		return;
	
	} else {
		
		/* If the Instruction is not correct */
		printf("-------------------\n");
		printf("INVALID INSTRUCTION!\nUse only Load or Convert for Instruction (Case Sensitive)\n");
		printf("Cancelling Code...\n");
		printf("-------------------\n");
		return;
	
	}
	
	return;
}
