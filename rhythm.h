#include <stm32f10x.h>
#include <stdlib.h>

//Describe a boolean value
typedef enum { false, true } bool;

#define MAX_BLOCK_NUM 8
#define MAX_PATTERN_SIZE 14

//Declare Global Functions 
extern void create_block(u8);
extern void create_rand_block(void);
extern void write_numeral(char,u8);
extern void test_display(void);
extern void clear_display(void);
extern void display_menu(void);
extern void write_to_display(void);
extern void load_game_screen(void);
extern void clear_block_screen(void);
extern void score(char,u8); // H for hit L for miss
extern void clear_score_screen(void);
extern void micro_sec_delay(void);
extern void milli_sec_delay(u8);
extern void light_led(u8);
extern void turn_off_led(u8);

//Declare Global Variables
volatile extern u32 seed;
//Declare Global Arrays
volatile extern u16 DOT_COL_G[16]; 
volatile extern u16 DOT_COL_R[16];

//Struct describing a block
struct block{
	u8 row; //dot matrix row position 16 0 if inactive
	char color; //Either 'R' or 'G'
	u16 pattern; //The pattern of the block, determined by its block column 0x0003
};

//Game configuration
struct gameConfig {
	u8 level;
	u8 speed;
	char page; //'M' for menu or 'G' for game or 'R' for reset
	bool pauseSwitch; //turn on for pause,off for off
	bool randomPattern; //turn on for random off for off
	u8 pressedKey; //index integer of the key from 1~16
	u8 noOfBlocks; //counts the number of blocks on the screen
	u8 hitScore; //hit score variable
	u8 missScore; //miss score variable
	u8 hitRatio; //hit ratio will be calculated
	u8 previousBlockIndex; //the index on the block buffer of the previous block
};

volatile extern struct gameConfig rhythm;
volatile extern struct block blockBuffer[MAX_BLOCK_NUM]; //Make an array of blockBuffers
