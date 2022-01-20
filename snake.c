#include "snake.h"

void draw_apple(int seed1, int seed2)
{
    if (!isAnAppleOnTheScreen)
    {
        appleX = rand (5*seed1, 12*seed2, GRID_SIZE_X);
        appleY = rand (4*seed1, 7*seed2, GRID_SIZE_Y);
        while (grid[appleX][appleY] > 0){
            seed1 += 17;
            seed2 += 3;
            appleX = rand (5*seed1, 12*seed2, GRID_SIZE_X);
            appleY = rand (4*seed1, 7*seed2, GRID_SIZE_Y);
        }

        grid[appleX][appleY] = -1;
        isAnAppleOnTheScreen = 1;
    }
	draw_rectangle(snake_to_lcd(appleX) + APPLE_OFFSET, snake_to_lcd(appleY) + APPLE_OFFSET, CELL_SIZE - 2 * APPLE_OFFSET, CELL_SIZE - 2 * APPLE_OFFSET, LCDWhite);
}


void config_Timer()
{
	LPC_TIM0->PR = 0;
	LPC_TIM0->MR0 = SystemCoreClock / 16;
	LPC_TIM0->MCR = 3;
	LPC_TIM0->TCR = 1;
	NVIC_EnableIRQ(TIMER0_IRQn);
}

void move_snake(enum MovementDirection direction)
{
        int target_x;
        int target_y;
        switch (direction)
        {
            case LEFT:
                target_x = snake_head_x - 1;
                target_y = snake_head_y;
                break;
            case RIGHT:
                target_x = snake_head_x + 1;
                target_y = snake_head_y;
                break;
            case UP:
                target_x = snake_head_x;
                target_y = snake_head_y + 1;
                break;
            case DOWN:
                target_x = snake_head_x;
                target_y = snake_head_y - 1;
                break;
        }
        
        grid[target_x][target_y] = snake_size;
        snake_head_x = target_x;
        snake_head_y = target_y;
        
        if (grid[snake_head_x][snake_head_y] > 0){
            play_sound(lose, LOSE_LEN);
            areYouWinningSon = 0;
        }

        if (grid[snake_head_x][snake_head_y] == -1)
        {
            snake_size += 1;
            play_sound (win, WIN_LEN);
        }
        //else
        {
            //stwórz nowy segment w odpowiednim kierunku o wartości snake_size
            for (int i = 0; i < GRID_SIZE_X; i++)
                for (int j = 0; j < GRID_SIZE_Y; j++)
                {
                    if (grid[i][j] == 1)
                        draw_snake_part(i, j, LCDBlack);
                    if (grid[i][j] > 0)
                        grid[i][j] -= 1;
                }
        }
        
        // fill_background(LCDBlack);
        for (int i = 0; i < GRID_SIZE_X; i++)
            for (int j = 0; j < GRID_SIZE_Y; j++)
            {
                if (grid[i][j] > 0)
                    draw_snake_part(i, j, LCDWhite);				
            }
        //wygeneruj i narysuj nowe jabłko
        //play_sound(win, WIN_LEN);
        //play_sound(lose, LOSE_LEN);
}


void TIMER0_IRQHandler(void)
{
    if (areYouWinningSon)
    {
        int *x;
        int *y;
        int sample_x, sample_y;
        *x = 0;
        *y = 0;
        for (int i = 0; i < SAMPLE_SIZE_INGAME; i++)
        {
            touchpanelGetXY(&sample_x, &sample_y);
            *x += sample_x;
            *y += sample_y;
        }
        *x /= SAMPLE_SIZE_INGAME;
        *y /= SAMPLE_SIZE_INGAME;
        if (*x > 100 && *x < 3500)
        {
            
        touch_x = *x;
        //touch_x = last_touch_x;
        //touch_y = last_touch_y;
        //touch_to_lcd(&touch_x, &touch_y);
        //touch_x = lcd_to_snake(touch_x);
        //touch_y = lcd_to_snake(touch_y);
        
        //if (touch_x < snake_head_x && touch_y > snake_head_y - touch_x && touch_y < snake_head_y + touch_x)
        //	direction = LEFT;
        //else if (touch_x > snake_head_x && touch_y > snake_head_y - (touch_x - snake_head_x) && touch_y < snake_head_y + (touch_x - snake_head_x))
        //	direction = RIGHT;
        //else if (touch_y < snake_head_y)
        //	direction = UP;
        //else
        //	direction = DOWN;
    //	move_snake(direction);
        
        switch (direction){
            case LEFT : 
            {
                if (touch_x <= 2000/2)
                    direction = UP;
                else
                    direction = DOWN;
                break;
            }
            case RIGHT :
            {
                if (touch_x <= 2000/2)
                    direction = DOWN;
                else
                    direction = UP;
                break;
            }
            case UP :
            {
                if (touch_x <= 2000/2)
                    direction = RIGHT;
                else
                    direction = LEFT;
                break;
            }
            case DOWN :
            {
                if (touch_x <= 2000/2)
                    direction = LEFT;
                else
                    direction = RIGHT;
                break;
            }
        }
        }
        move_snake (direction);
        draw_apple (sample_x, sample_y);
        
        LPC_TIM0->IR = 1;
    }
}
