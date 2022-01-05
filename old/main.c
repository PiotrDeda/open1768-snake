#include "Board_Buttons.h"              // ::Board Support:Buttons
#include "sound.h"
//#include "snakeSounds.h"
#include "win.h"
#include "lose.h"

void config()
{
	config_sound();
}

int main()
{ 
	config();
	
	while(1)
	{
		if (Buttons_GetState() == 1)
			play_sound(win, 4095);
		if (Buttons_GetState() == 2)
			play_long_sound(lose, LOSE_LEN);
	}
}
