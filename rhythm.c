#include "rhythm.h"

u16 activeRow, //this is the active row
keyRow, keyCol, prevKey;

u8 activeRowCount, //simpler tracking the active row
colCount,
j, //Random counter for the main function
blockCreationRate, //Used to regulate block spacing
keyCount, // Indicates current row(0~3) of key matrix
perfect, good, fault;

int patternTracker; //I use this to keep track on the pattern array

//block patterns
u8 pattern1[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0x82, 0, 0, 0, 0x12, 0, 0, 0, 0x81, 0, 0x41, 0, 0x21, 0, 0x11, 0, 
		0x12, 0, 0, 0, 0x82, 0, 0, 0, 0x11, 0, 0x21, 0, 0x41, 0, 0x81, 0, 
		0x92, 0, 0, 0, 0x62, 0, 0, 0, 0x91, 0, 0x61, 0, 0x61, 0, 0x91, 0, 
		0x82, 0, 0x42, 0, 0x22, 0, 0x12, 0, 0, 0x11, 0x21, 0x41, 0x81, 0x41, 0x21, 0x11, 
		0x11, 0, 0x81, 0, 0x61, 0, 0, 0x11, 0, 0x21, 0, 0x41, 0, 0x81, 
		0x82, 0, 0x12, 0, 0x62, 0, 0, 0x82, 0, 0x42, 0, 0x22, 0, 0x12, 
		0x12, 0, 0x82, 0, 0x62, 0, 0, 0x12, 0, 0x22, 0, 0x42, 0, 0x82, 
		0x81, 0, 0x11, 0, 0x61, 0, 0, 0x91, 0, 0x11, 0x21, 0x41, 0x81, 0x41, 0x21, 0x11, 
		0x11, 0, 0x81, 0, 0x61, 0, 0, 0x11, 0, 0x21, 0, 0x41, 0, 0x81, //Far in the distant future
		0x82, 0, 0x12, 0, 0x62, 0, 0, 0x82, 0, 0x42, 0, 0x22, 0, 0x12, //Beyond the pages of
		0x12, 0, 0x82, 0, 0x62, 0, 0, 0x12, 0, 0x22, 0x42, 0, 0x82, 0, 0, 0, //our time ~~~~
		0x11, 0, 0x81, 0, 0x61, 0, 0, 0x11, 0, 0x21, 0, 0x41, 0, 0x81, //Cold blooded wicked tyrants
		0x82, 0, 0x12, 0, 0x62, 0, 0, 0x82, 0, 0x42, 0, 0x22, 0, 0x12, //Threaten the freedom of
		0x12, 0, 0x82, 0, 0x62, 0, 0, 0x12, 0, 0x22, 0x42, 0x42, 0x82, 0x82, 0x12, 0x12, //man kind ~~~~
		0x11, 0, 0x81, 0, 0x61, 0, 0, 0x11, 0, 0x21, 0, 0x41, 0, 0, //Corruption, lust, and greed
		0x82, 0, 0x12, 0, 0x62, 0, 0, 0x82, 0, 0x42, 0, 0x22, 0, 0x12, //defy the new nobility.
		0x11, 0, 0, 0, 0x41, 0, 0x81, 0x11, 0x81, 0x41, 0x21, 0, 0, 0, //Changing
		0, 0x51, 0, 0x51, 0, 0xA1, 0, 0xA1, 0, 0x61, 0, 0x11, 0x21, 0x41, 0x81, 0x41, 0x21, 0x11, //across the history ~~~~
		0x11, 0, 0x81, 0, 0x61, 0, 0, 0x11, 0, 0x21, 0, 0x41, 0, 0x81, //Across the vast North Empire
		0x82, 0, 0x12, 0, 0x62, 0, 0, 0x82, 0, 0x42, 0, 0x22, 0, 0x12, //Most people struggle to sur-
		0x12, 0, 0x82, 0, 0x62, 0, 0, 0xC2, 0, 0, 0, 0x32, 0, 0, 0, 0, //-vive ~~~~
		0x11, 0, 0x81, 0, 0x61, 0, 0, 0x11, 0, 0x21, 0, 0x41, 0, 0x81, //Living a meek existence
		0x82, 0, 0x12, 0, 0x62, 0, 0, 0x82, 0, 0x42, 0, 0x22, 0, 0x12, //While their scheduled lead-
		0x61, 0, 0x91, 0, 0, 0, 0, 0, 0, 0x12, 0x82, 0, 0x42, 0, 0, 0, 0x12, 0x22, 0x42, 0, 0, 0, //-er thrives ~~~~
		0, 0, 0, 0, 0x81, 0, 0, 0, 0x41, 0, 0x21, 0, 0x11, 0, 0, 0, 0, 0, 0, 0, 0x91, 0, 0, 0, 0, 0, 0x12, 0x22, 0x42, 0, 0, 0, //We are living day to day
		0, 0, 0x82, 0, 0x82, 0x42, 0x22, 0, 0x12, 0, 0x42, 0, 0x62, 0, 0, 0, 0, 0, 0, 0x91, 0x91, 0, 0x91, 0, 0x61, 0x61, 0, 0, //Forced to bear the lion's share
		0x91, 0, 0, 0, 0, 0, 0, 0, 0x82, 0, 0, 0, 0x21, 0, 0, 0, //People just don't have the time for
		0x51, 0, 0, 0, 0, 0, 0, 0, 0xA1, 0, 0, 0, 0x81, 0x41, 0x11, 0x41, //music anymore but
		0x61, 0, 0, 0, 0, 0, 0, 0x12, 0x82,  0, 0, 0, 0x12, 0x82, 0x12, 0x82, //no one seems to care Our
		0x22, 0, 0, 0x42, 0, 0x42, 0x22, 0, 0x11, 0x41, 0x21, 0x11, //My friend has seen the chosen one
		0x22, 0, 0, 0x42, 0, 0x42, 0x82, 0x41, 0x21, 0x11, 0, 0, //Our quest for freedom has begun
		0x91, 0, 0, 0x61, 0, 0x61, 0x91, 0, 0, 0x41, 0, 0x21, 0x62, 0, 0, 0x92, 0, 0, 0x11, 0x81, 0x11, 0x81, 0x21, 0x41, //He will be the answer to out prayers
		0x11, 0, 0x81, 0, 0x61, 0, 0, 0x11, 0, 0x21, 0, 0x41, 0, 0x81, //There walks a god among us
		0x82, 0, 0x12, 0, 0x62, 0, 0, 0x82, 0, 0x42, 0, 0x22, 0, 0x12, //Who's seen the writing on the
		0x12, 0, 0x82, 0, 0x62, 0, 0, 0x12, 0, 0, 0, 0, 0x42, 0, 0, 0, //wall ~~~~
		0x11, 0, 0x81, 0, 0x61, 0, 0, 0x11, 0, 0x21, 0, 0x41, 0, 0x81, //He is the revolution
		0x82, 0, 0x12, 0, 0x62, 0, 0, 0x82, 0, 0x42, 0, 0x22, 0, 0x12, //He'll be the one to save us
		0x12, 0, 0x82, 0, 0x62, 0, 0, 0, 0x22, 0x42, 0x82, 0x42, 0x82, 0x42, 0x22, 0x12, //all ~~~~
		0x91, 0, 0x81, 0, 0x81, 0, 0, 0x41, 0, 0, 0, 0x61, 0, 0, 0, 0x21, //My brother Gabriel, is
		0x12, 0, 0x12, 0, 0x92, 0, 0, 0, 0x82, 0, 0, 0, 0x12, 0, 0, 0, //all the hope we need 
		0x91, 0, 0x81, 0x11, 0, 0x61, 0, 0x21, 0x42, 0, 0, 0, 0x22, 0, 0, 0, //Shining like a beacon in the
		0x92, 0, 0, 0, 0xA2, 0, 0, 0, 0x52, 0, 0, 0, 0x92, 0, 0, 0, //sky ~~~~
		0x91, 0, 0x81, 0, 0x81, 0, 0, 0x41, 0, 0, 0, 0x61, 0, 0, 0, 0x21, //Shepred of Ravenskill, 
		0x12, 0, 0x12, 0, 0x92, 0, 0, 0, 0x82, 0, 0, 0, 0x12, 0, 0, 0, //a reason to believe
		0x21, 0x21, 0, 0x81, 0, 0x81, 0, 0x11, 0, 0x11, 0, 0x91, 0, 0, 0, 0x22, //Music is the gift he brings The
		0x22, 0, 0x42, 0, 0x42, 0, 0, 0x82, 0, 0xC2, 0, 0x12, 0, 0x32, 0, 0, 0, 0, //songbird stops to listen when he
		0x91, 0, //sings ~~~~
		0xff};

volatile u16 DOT_COL_G[16] = {0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000};
volatile u16 DOT_COL_R[16] = {0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000};
static u8 bpm[10] = {0,158,40,60,80,100,120,160,200,240};

volatile struct gameConfig rhythm;
volatile struct block blockBuffer[MAX_BLOCK_NUM];

static struct block emptyBlock;

int main(void) {

	//GPIO PIN CONFIG
	//peripheral clock enable TIM1,PA,PB,PC
	RCC->APB2ENR = 0x0000081D;
	AFIO->MAPR |= 0x04000400; //disable JTAG 
	GPIOA->CRL = 0x33333333; //DOT MATRIX ROW
	GPIOA->CRH = 0x33333333; //DOT MATRIX ROW
	GPIOB->CRL = 0x33333033; //KEY_CS,R_CS,G_CS
	GPIOB->CRH = 0x44443333; //key pad
	GPIOC->CRL = 0x33333333; //DOT MATRIX COLUMN
	GPIOC->CRH = 0x33333333; //DOT MATRIX COLUMN
	
	//peripheral clock enable TIM2,TIM3
	RCC->APB1ENR = 0x00000003;
	
	//DISPLAY TIMER 1 CONFIG
	TIM1->CR1 = 0x00;
	TIM1->CR2 = 0x00;
	TIM1->PSC = 0x0002; 
	TIM1->ARR = 0x2000;
	TIM1->DIER = 0x0001;
	NVIC->ISER[0] |= (1<<25);
	
	
	//KEY SCANNING TIMER
	TIM2->CR1 = 0x00;
	TIM2->CR2 = 0x00;
	TIM2->PSC = 0x00C7; //30Hz
	TIM2->ARR = 0x03E7;
	TIM2->DIER = 0x0001;
	NVIC->ISER[0] |= (1<<28); 

	//GAME PLAY TIMER
	TIM3->CR1 = 0x00;
	TIM3->CR2 = 0x00;
	TIM3->PSC = 0xffff; //2Hz
	TIM3->ARR = 0x00b6;
	TIM3->DIER = 0x0001;
	NVIC->ISER[0] |= (1<<29);
	
	//Enable the software interrupt
	NVIC->ISER[1] |= (1 << 8);
	EXTI->IMR |= 0x0000F000;
	AFIO->EXTICR[3] = 0x00001111;
	//	EXTI->FTSR = 0x0000f000;
	
	//Initialize variables
	activeRow = 0x8000; //set active row to bottom
	colCount = 0;
	activeRowCount = 0;
	blockCreationRate = 0;
	patternTracker = 0;
	
	keyRow = 0x01;
	keyCount = 0;
	keyCol = 0;
	
	emptyBlock.color = 'R';
	emptyBlock.row = 0x00;
	emptyBlock.pattern = 0x0000;
	//initialize the blocks
	for (j=0;j<MAX_BLOCK_NUM;++j) blockBuffer[j] = emptyBlock;
	
	//Set up the game configuration
	rhythm.hitScore = 0;
	rhythm.level = 1;
	rhythm.speed = 1;
	rhythm.missScore = 0;
	rhythm.noOfBlocks = 0;
	rhythm.hitRatio = 0;
	rhythm.page = 'M';
	rhythm.pressedKey = 0; //reset to null
	rhythm.pauseSwitch = false; //bool
	rhythm.randomPattern = false; //bool
	rhythm.previousBlockIndex = 0;
	
	//Clear the display
	clear_display();
	write_to_display(); //execute the reset
	//test_display();
	turn_off_led(0);
	display_menu(); //display the menu
	write_numeral('L',rhythm.level);
	write_numeral('S',rhythm.level);
	//Enable the timers to start the count
	TIM1->CR1 |= 0x0001; //display timer
	TIM2->CR1 |= 0x0001; //keyscaning timer
	//manually enable the game timer in the interrupt handler
	
	while(1){;}  //endless loop
}

void TIM1_UP_TIM16_IRQHandler(void) { //#25
	if ((TIM1->SR & 0X0001)!=0){
		GPIOA->ODR = activeRow;
		activeRow>>=1; //move to the next active row
		//set the green col
		//GPIOC->ODR = ~(DOT_COL_G[colCount]);
		//GPIOB->ODR |= 0x00000002; //Green CS on
		//GPIOB->ODR &= ~(0x00000002); //Green CS off
		//set the red col and increment the column count
		GPIOC->ODR = ~(DOT_COL_R[colCount++]);
		GPIOB->ODR |= 0x00000001; //Red CS on
		GPIOB->ODR &= ~(0x00000001); //Red CS off
		//reset the col_count if 16
		if(colCount == 16) colCount = 0;
		//reset the active row if at the end
		if(activeRow==0) activeRow = 0x8000;
		TIM1->SR &= ~(1<<0); //clear UIF
	}
}

//Try triggerring different functions
void TIM2_IRQHandler(void) { //#28
	u16 mask, key_change;
	if(keyRow == 0x0008) {
		keyRow = 0x0001;
		keyCount = 0;
	}
	else {
		keyRow <<= 1;
		keyCount ++;
	}
	
	GPIOB->ODR &= ~0x0f00;
	GPIOB->ODR |= (~keyRow << 8) & 0x0f00;	//KEY detection
	GPIOB->ODR |= 0x0080; //KEY_CS
	GPIOB->ODR &= ~0x0080;
	keyCol = (~GPIOB->IDR & 0xf000) >> 12;
	
	mask = (prevKey >> 4*keyCount) & 0x000f;
	key_change = mask ^ keyCol;
	
	if(key_change) {
		if(key_change & ~mask) {
			EXTI->SWIER = (key_change & ~mask) << 12;
		}
	}
	
	prevKey &= ~(0xf << 4*keyCount);
	prevKey |= keyCol << 4*keyCount;
	
	TIM2->SR &= ~(1<<0); //clear UIF
}


//This timer handle block creation and movement
void TIM3_IRQHandler(void) { //#29
	if ((TIM3->SR & 0X0001)!=0){
		if(rhythm.page=='G'){
			int i;
			if(blockCreationRate<(rhythm.speed)) //we create our regulating rate
				blockCreationRate++;
			else {
				blockCreationRate = 0; //reset the blockCreation Rate
				if (rhythm.randomPattern==true) 
					create_rand_block(); //no need for pattern
				else {
					if(pattern1[patternTracker]!=0xff)
						create_block(pattern1[patternTracker++]); //This will create a block in the global block buffer
				}
			}
			clear_block_screen(); //clear the blocks
			//We loop through the no of blocks in 
			for(i=0;i<MAX_BLOCK_NUM;++i){
				if(blockBuffer[i].row<16&&blockBuffer[i].pattern!=0){ //0 pattern means no block
					//draw the blocks to the display buffers
					if(blockBuffer[i].color=='R')
						DOT_COL_R[blockBuffer[i].row++] |= 0x0030<<(blockBuffer[i].pattern*2-2); //reset the column values for red
					if(blockBuffer[i].row==16){
						if(rhythm.hitScore<255) rhythm.missScore++;
						rhythm.hitRatio = (rhythm.hitScore*15/(rhythm.missScore+rhythm.hitScore));
						clear_score_screen();
						score('H',rhythm.hitRatio);
						turn_off_led(0x01<<(blockBuffer[i].pattern-1));
					}
					//else
						//DOT_COL_G[blockBuffer[i].row--] |= 0x0300<<(blockBuffer[i].pattern-1); //reset the column values for green
				}
			}
		}
		TIM3->SR &= ~(1<<0); //clear UIF
	}
}

//Interrupt handler for key press events
void EXTI15_10_IRQHandler(void) {
	u8 blockColumnNumber = 0;
	u16 temp; //temporary variable for multiple uses
	int hitFlag = 0;
	int i;
	if(EXTI->PR & 0x00008000) {
		switch(keyCount) {
			case 0 : //Reset
				rhythm.page = 'R';
				break;
			case 1 : //Pause
				rhythm.page = 'P';
				if(rhythm.pauseSwitch)
					rhythm.pauseSwitch = false;
				else
					rhythm.pauseSwitch = true;
				break;
			case 3 : //Block column 1
				blockColumnNumber |= 0x01;
				break;
		};
	}	
	if(EXTI->PR & 0x00004000) {
		switch(keyCount) {
			case 0 : //Game Start
				for (i=0;i<MAX_BLOCK_NUM;++i) blockBuffer[i] = emptyBlock;
				rhythm.hitScore = 0;
				rhythm.missScore = 0;
				rhythm.noOfBlocks = 0;
				rhythm.pauseSwitch = false;
				rhythm.previousBlockIndex = 0;
				rhythm.randomPattern = false;
				patternTracker = 0;
			
				clear_display();
				load_game_screen();
				turn_off_led(0);
			
				rhythm.page = 'G';
				TIM3->PSC = ((60*24000000)/((0x00b6+1)*2*(rhythm.speed+1)*bpm[rhythm.level]))-1;
				TIM3->CR1 |=0x0001; //turn on game timer
				break;
			case 1 : //Toggle Random Block Generation
				if(rhythm.page=='G'){
					if(rhythm.randomPattern==true)rhythm.randomPattern = false;
					else rhythm.randomPattern = true;
				}
				break;
			case 3 : //Block column 2
				blockColumnNumber |= 0x02;
				break;
		};
	}	
	if(EXTI->PR & 0x00002000) {
		switch(keyCount) {
			case 0 : //Level Up
				if(rhythm.page=='M'){
					if(rhythm.level<9)rhythm.level++;
					write_numeral('L',rhythm.level);
				}
				break;
			case 1 : //Level Down
				if(rhythm.page=='M'){
					if(rhythm.level>1)rhythm.level--;
					write_numeral('L',rhythm.level);
				}
				break;
			case 3 : //Block column 3
				blockColumnNumber |= 0x04;
				break;
		};
	}
	if(EXTI->PR & 0x00001000) {
		switch(keyCount) {
			case 0 : //Speed Up
				if(rhythm.page=='M'){
					if(rhythm.speed<9)rhythm.speed++;
					write_numeral('S',rhythm.speed);
				}
				break;
			case 1 : //Speed Down
				if(rhythm.page=='M'){
					if(rhythm.speed>1)rhythm.speed--;
					write_numeral('S',rhythm.speed);
				}
				break;
			case 3 : //Block column 4
				blockColumnNumber |= 0x08;
				break;
		};
	}

	//This switch statement will handle our game mode executions
	switch(rhythm.page){
		//Perform menu functions
		case 'M':
			//select options for menu
			//Can add extra functions
			turn_off_led(0);
			break;
		case 'G':
			//Perform Game functions
			if(!rhythm.pauseSwitch){
				if(blockColumnNumber!=0){
					turn_off_led(0);
					for (i=0;i<MAX_BLOCK_NUM;++i) {
						if(blockBuffer[i].pattern!=0x0000&&blockBuffer[i].row<15&&blockBuffer[i].row>11){
							temp = 0x01<<(blockBuffer[i].pattern-1);
							if((temp==(blockColumnNumber&0x01))||(temp==(blockColumnNumber&0x02))||
								(temp==(blockColumnNumber&0x04))||(temp==(blockColumnNumber&0x08)))
							{
								rhythm.noOfBlocks--;
								if(rhythm.hitScore<255) rhythm.hitScore++;
								light_led(blockColumnNumber);
								blockBuffer[i] = emptyBlock;
								hitFlag = 1;
							}
						}
					}
					if (hitFlag==0) {
						if(rhythm.hitScore<255) rhythm.missScore++;
						turn_off_led(blockColumnNumber);
					}
					rhythm.hitRatio = (rhythm.hitScore*15/(rhythm.missScore+rhythm.hitScore));
					clear_score_screen();
					score('H',rhythm.hitRatio);
				}
			}
			break;
		case 'R': //Reset mode
			//turn off the game timer and only show the menu
			TIM3->CR1 &= ~(0x0001);
			//reset game configuration
			rhythm.hitScore = 0;
			rhythm.level = 1;
			rhythm.speed = 1;
			rhythm.missScore = 0;
			rhythm.page = 'M';
			rhythm.pauseSwitch = false; //bool
			rhythm.previousBlockIndex = 0;
			patternTracker = 0;
		
			clear_display();
			display_menu();
			turn_off_led(0);
			write_numeral('L',rhythm.level);
			write_numeral('S',rhythm.level);
			for (i=0;i<MAX_BLOCK_NUM;++i) blockBuffer[i] = emptyBlock;
			break;
		case 'P': //Pause mode
			//turn off the game timer
			if(rhythm.pauseSwitch) 
				TIM3->CR1 &= ~(0x0001); //turn off the timer
			else{
				TIM3->CR1 |=0x0001; //turn on game timer
				rhythm.page = 'G';
			}
			break;
	};
	EXTI->PR |= 0x0000f000;
}

