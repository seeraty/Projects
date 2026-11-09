  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #
.data
.globl delay, time2string

.text
hexasc:
# only for digits from hexmain.asm
	li $t2,0x30	
	add $v0,$t2,$a2
	andi $v0,$v0,0x7f	#return value

	jr $ra
	nop
	
delay:
	PUSH $ra	
	addi $t0,$a0,0	#copy the parameter value ms into $t0

while:
	beqz $t0,exit		# exit loop condition for while
	li $t2,6000		# change accordingly depending on delay		 
	li $t1,0

for:
# executing the for loop takes approximately 1 ms
	beq $t1,$t2,endfor	#exit loop condition 
	nop
	addi $t1,$t1,1		# increment i by 1
	j for
	nop

endfor:
# decrement the while-loop counter by 1
	sub $t0,$t0,1	# ms = ms - 1	
	j while
	nop
exit:

	POP $ra
	jr $ra
	

time2string:
#$a0 contains the base address of area in memory
#imagine an array[i] of size, base address points to the start of the array
#offset number used to access elements of array
#$v0 contains the ASCII-coded digit from hexasc function
	 PUSH $ra
	 PUSH $a1
	 PUSH $a0
	#get 1 second  (ax)
	andi $a2,$a1,0x0f    #get the lsb 
  	  jal hexasc 
  	  nop
  	  move $t4, $v0
    
  	  # get 2 second  (xa)
 	   srl $a1,$a1,4    #shift 4 steps to the right
	andi $a2,$a1,0x0f    
  	  jal hexasc 
	    nop
  	  move $t5, $v0
      
      # see if they are both 0. Start with the second digit (x0)
      beq $t5,0x30, compare
      nop
      j time
      compare: 
      beq $t4,0x30, makeDing
    nop
    j time
    makeDing:
    li $t1,0x44 #D
	sb $t1,0($a0)
li $t1,0x49 #I
sb $t1,1($a0)
li $t1,0x4e #N
sb $t1,2($a0)
li $t1,0x47 #G
sb $t1,3($a0)
li $t1,0x00 #null-byte character
sb $t1,4($a0)
j exit2
    
time:
    # If seconds were not 0
      sb $t4,4($a0) #store the right-most digit
sb $t5,3($a0)
 
li $t1,0x3A #colon character
sb $t1,2($a0)
 
srl $a1,$a1,4   
andi $a2,$a1,0x0f    
    jal hexasc 
    nop
      sb $v0,1($a0)
 
srl $a1,$a1,4   
andi $a2,$a1,0x0f    
    jal hexasc 
    nop
      sb $v0,0($a0)
       
      li $t1,0x00 #null-byte character
sb $t1,5($a0)

j exit2
 
# reset registers
exit2: 
POP $a0
POP $a1
POP $ra
jr $ra
nop

