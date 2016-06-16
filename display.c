#include "rhythm.h"

#define MAX_BLOCK_NUM 8

int i;

//Menu pattern
// LVL X
// SPD X
// 00
// 00
u8 levelNumerals[10] = {0x00,0x40,0x40,0x40,
0x60,0x60,0x60,
0x70,0x70,0x70
};
u8 speedNumerals[10] = {0x00,0x04,0x04,0x04,
0x06,0x06,0x06,
0x07,0x07,0x07
};
u16 menu[16] = {0x0071,0x0017,0x0014,0x0000, //L - S
0x0067,0x0016,0x0060,0x0000, //V - P
0x0077,0x0015,0x0012,0x0000, //L - D
0x0000,0x0000,0x0000,0x0000
};
u16 gameBackground[16] = {0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,
0x0000,0xff00,0x0000,0x00f0
};

//@params: 8 bit block configuration
//return void
void create_block(u8 blockConfig){
	u8 mask,i;
	struct block temp;
	if(blockConfig&0xf0){
		mask = 0x80;
		for(i=0;i<4;++i){
			if (blockConfig&mask) {
				temp.color = 'R';
				temp.row = 0;
				//if the number of blocks does not exceed the max amount increment
				switch(mask){
					case 0x80: temp.pattern = 1; break;
					case 0x40: temp.pattern = 2; break;
					case 0x20: temp.pattern = 3; break;
					case 0x10: temp.pattern = 4; break;
					default: temp.pattern = 0; break;
				}
				if(rhythm.previousBlockIndex<MAX_BLOCK_NUM);
				else
					rhythm.previousBlockIndex = 0;
				rhythm.noOfBlocks++;
				blockBuffer[rhythm.previousBlockIndex++] = temp; //allocate to struct position 0
			}
			mask>>=1;
		}
	}
}

//Creates a random block
//@param: u8 random number .. maybe clock count to seed the randomness
void create_rand_block(void){
	u8 seed,i,blocksPerRow; //clock count or something
	/* Intializes random number generator */
	u32 randFactor;
	struct block temp;
	
	seed = rand(); //seed with random number
	if (seed>50) blocksPerRow = 1;
	else blocksPerRow = 2;
	randFactor = seed + 271828192;
	randFactor *= 314159; //randomize a bit more
	randFactor >>= (seed&0x0f) ;
	randFactor &= 0x000000ff;
	for(i=0;i<blocksPerRow;++i){
		temp.color = 'R';
		
		if (randFactor>0) temp.pattern = 1+i;
		if (randFactor>64) temp.pattern = 2+i;
		if (randFactor>128) temp.pattern = 3-i;
		if (randFactor>192) temp.pattern = 4-i;
		
		temp.row = 0;
		//if the number of blocks does not exceed the max amount increment
		if(rhythm.previousBlockIndex<MAX_BLOCK_NUM);
		else
			rhythm.previousBlockIndex = 0;
		rhythm.noOfBlocks++;
		blockBuffer[rhythm.previousBlockIndex++] = temp; //allocate to struct position 0
	}
}

//This function displays the menu
void display_menu(void){
	int i;
	for(i=0;i<16;++i) DOT_COL_R[i] = menu[i]; //copy the menu pattern
};

//This function clears the display
void clear_display(void){
	int i;
	//make the whole display null
	for(i=0;i<16;++i) {
		DOT_COL_G[i] = 0x0000;
		DOT_COL_R[i] = 0x0000;
	}
};

//This function tests the display pixels
void test_display(void){
	int i;
	//make a test display of black and red stripes
	for(i=0;i<16;++i) {
		//DOT_COL_G[i] = 0x0f0f;
		DOT_COL_R[i] = 0xf0f0;
	}
};

//manually write the colour matrices to the display
void write_to_display(void){
	u16 activeRow;
	u8 colCount;
	for(colCount=0,activeRow=0x8000;activeRow>0;activeRow>>=1,++colCount){
		GPIOA->ODR = activeRow;
		activeRow>>=1; //move to the next active row
		//set the green col
		GPIOC->ODR = ~(DOT_COL_G[colCount]);
		GPIOB->ODR |= 0x00000001; //Green CS on
		GPIOB->ODR &= ~(0x00000001); //Green CS off
		//set the red col and increment the column count
		GPIOC->ODR = ~(DOT_COL_R[colCount]);
		GPIOB->ODR |= 0x00000002; //Red CS on
		GPIOB->ODR &= ~(0x00000002); //Red CS off
	}
}

//@params: char denoting menu item, integer
void write_numeral(char menu,u8 numeral){
	int i;
	//The columns for numerals are from index 11-15
	if (menu == 'L') { //level menu item
		for(i=12;i<16;++i) DOT_COL_R[i] &= 0xff0f; //make the numerals 0
		if(numeral>0) DOT_COL_R[12] |= levelNumerals[1];
		if(numeral>1) DOT_COL_R[13] |= levelNumerals[2];
		if(numeral>2) DOT_COL_R[14] |= levelNumerals[3];
		if(numeral>3) DOT_COL_R[12] |= levelNumerals[4];
		if(numeral>4) DOT_COL_R[13] |= levelNumerals[5];
		if(numeral>5) DOT_COL_R[14] |= levelNumerals[6];
		if(numeral>6) DOT_COL_R[12] |= levelNumerals[7];
		if(numeral>7) DOT_COL_R[13] |= levelNumerals[8];
		if(numeral>8) DOT_COL_R[14] |= levelNumerals[9];
	}
	else { //speed menu item
		for(i=12;i<16;++i) DOT_COL_R[i] &= 0xfff0; //make the numerals 0
		if(numeral>0) DOT_COL_R[12] |= speedNumerals[1];
		if(numeral>1) DOT_COL_R[13] |= speedNumerals[2];
		if(numeral>2) DOT_COL_R[14] |= speedNumerals[3];
		if(numeral>3) DOT_COL_R[12] |= speedNumerals[4];
		if(numeral>4) DOT_COL_R[13] |= speedNumerals[5];
		if(numeral>5) DOT_COL_R[14] |= speedNumerals[6];
		if(numeral>6) DOT_COL_R[12] |= speedNumerals[7];
		if(numeral>7) DOT_COL_R[13] |= speedNumerals[8];
		if(numeral>8) DOT_COL_R[14] |= speedNumerals[9];
	}
};

//Loads the game screen background
void load_game_screen(void){
	int i;
	//Make the display into the game display
	for(i=0;i<16;++i) {
		//DOT_COL_G[i] = gameBackground[i];
		DOT_COL_R[i] = gameBackground[i]>>4;
	}
}

//Clears the block part of the screen
void clear_block_screen(void){
	int i;
	//Make the display into the game display clear all parts of the block screen except the line
	for(i=0;i<16;++i) { //start form the end..
		//DOT_COL_G[i] &= 0x00ff; //clear that part of the screen for both red and green
		if(i!=13) DOT_COL_R[i] &= (0xf00f); //leave only the line
	}
}

//Clears the score part of the screen
void clear_score_screen(void){
	int i;
	//Make the display into the game display clear all parts of the block screen except the line
	for(i=0;i<16;++i) { //start form the end..
		//DOT_COL_G[i] &= 0x00ff; //clear that part of the screen for both red and green
		DOT_COL_R[i] &= ~0x000f; 
	}
}

//writes the score to the lower half of the screen
//@params: H / M for hit or miss, number of points to display
void score(char type,u8 points){
	int i;
	switch(type){
		case 'H':
			for (i=0;i<points;++i) DOT_COL_R[14-i] |= 0x0006; //in position of score
			break;
	};
}

//Microsecond delay, should be calibrated
void micro_sec_delay(void){
	int i;
	for(i=0;i<1000;++i);
}

//Milli second delay
void milli_sec_delay(u8 delay){
	int i;
	for(i=0;i<delay;++i) micro_sec_delay();
}

//Light led
void light_led(u8 column){
		GPIOB->ODR |= column<<3;
}

//Turn off all the leds
void turn_off_led(u8 number){
	if(number==0)
		GPIOB->ODR &= ~0x00000078; //0b0000 0000 0111 1000
	else
		GPIOB->ODR &= ~(number<<3);
}
