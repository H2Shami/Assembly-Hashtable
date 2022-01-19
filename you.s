//YOU: Insert the first SIZE/2 elements from randos into hash_table using linear probing or whatever hash method you want
//Hash_table is of size SIZE
//void asm_hash_insert(int *hash_table, int *randos, int SIZE);
.global asm_hash_insert
asm_hash_insert:
	PUSH {LR}
	PUSH {R4-R12}
	
	MOV R8, R0
	MOV R9, R1
	MOV R11, R2
	LSR R10, R2, #1		//Inserting SIZE/2 elements
	SUB R10, #1			//SIZE/2 - 1 because we start at i = 0
	MOV R5, #-4			//i = 0 (we increment at the start so it's the same thing)
	MOV R7, #0

insert:
	ADD R5, #4			//Integers are 4 bytes so we increment by 4 bytes instead of 1
	LDR R4, [R9, R5]	//Load element from randos
	MOV R0, R4
	MOV R1, R11
	BL modulus			//Modulus by SIZE/2-1 to get the key
	LSR R5, #2			//Divide by 4 so that we convert i*size_of_int to i
	MOV R0, R5
	MOV R1, R10
	CMP R5, R10			//Compare i to SIZE/2-1
	ROR R5, #30			//Multiply R5 by 4 to undo the divide
	BEQ return

probing:
	LDR R6, [R8, R7]
	CMP R6, #0
	BEQ store
	ADD R7, #4			//Move key over by 4 (Linear probing)
	BAL probing

store:
	STR R4, [R8, R7]
	BAL insert

return:
	POP {R4-R12}
	POP {PC}

//YOU: Return true or false based on if the key is in the table
//Hash_table is of size SIZE
//bool asm_hash_search(int *hash_table, int key, int SIZE); 
.global asm_hash_search
asm_hash_search:
	PUSH {LR}
	PUSH {R4-R12}

	MOV R4, R0
	MOV R5, R1
	MOV R6, R2
	MOV R8, #-4
	LSR R9, R6, #1
	SUB R9, #1

searching:
	ADD R8, #4
	LDR R7, [R4, R8]
	CMP R7, R5
	BEQ found
	MOV R0, #0
	LSR R8, #2
	CMP R8, R9
	BEQ done
	ROR R8, #30
	BAL searching

found:
	MOV R0, #1

done:
	POP {R4-R12}
	POP {PC}
