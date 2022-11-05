#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <string>
#include <string.h>
#include <vector>

#define BOX_WIDTH 13
#define BOX_HEIGHT 13
#define OFFSET 3
#define START_Y 33


#define N_BUTTON_WIDTH 12
#define N_BUTTON_HEIGHT 15

#define S_BUTTON_WIDTH 20
#define S_BUTTON_HEIGHT 15

#define KEYBOARD_X 100
#define KEYBOARD_Y 170

#define INPUT_STRING_SIZE 6

using namespace std;

enum square_t
{
    DEFAULT,
    CORRECT_POS,
    WRONG_POS,
    WRONG
};

class Wordle : public olc::PixelGameEngine
{
    public:
        Wordle() {
            sAppName = "Wordle";
        }

    public:
    
    vector<pair<string, int>> buttons ={
        {"A", 0}, {"B", 0}, {"C", 0}, {"D", 0}, {"E", 0}, {"F", 0}, {"G", 0}, {"H", 0}, {"I", 0}, {"J", 0}, {"K", 0}, {"L", 0}, {"M", 0}, {"N", 0}, {"O", 0}, {"P", 0}, {"Q", 0}, {"R", 0}, {"S", 0}, {"->", 1}, {"T", 0}, {"U", 0}, {"V", 0}, {"W", 0}, {"X", 0}, {"Y", 0}, {"Z", 0}, {"<-", 1}
    };
    
    string grid_letters[30] = {};
    int grid_idx = 0;
    
    vector<pair<string, square_t>> input_word = {
        {"H", CORRECT_POS},
        {"M", WRONG},
        {"O", WRONG_POS},
        {"L", CORRECT_POS},
        {"L", WRONG_POS},
    };
    string input_string[INPUT_STRING_SIZE] = {};
    
    square_t box_status[30];
    vector<string> word = {"H", "E", "L", "L", "O"};
    
    bool OnUserCreate() override {
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::BLACK);
        scanKeyboard();
        isEqual();
        DrawFrame();
        FillRect(GetMouseX(), GetMouseY(), 1, 1);
        return true;
    }
        
    void DrawFrame()
    {
        // Draw Game Frame elements
        DrawString(158, 10, "Wordle", olc::WHITE);
        DrawLine(0, START_Y-10, ScreenWidth(), START_Y-10, olc::DARK_GREY);

        int pos_x = ((ScreenWidth() - (BOX_WIDTH + OFFSET)*6)/2) - 5;
        int pos_y = START_Y + 10;
        int i = 0;
        int random_val;
        DrawKeyboard(KEYBOARD_X, KEYBOARD_Y);
        // Draw Grid  - should probably be function of it's own
        while(i < 30)
        {
            random_val = i % 5;
            pos_x += (BOX_WIDTH + 3);
            DrawBox(pos_x, pos_y, box_status[i]);
            DrawString(pos_x + 3, pos_y + 3, grid_letters[i], olc::WHITE);
            i++;
            if(!(i % 5))
            {
                pos_y += BOX_HEIGHT + 3;
                pos_x = ((ScreenWidth() - (BOX_WIDTH + OFFSET)*6)/2) - 5;
            }
        }

    }
    
    
    void DrawBox(int pos_x, int pos_y, square_t box_type)
    {
        switch (box_type) {
            case DEFAULT:
                DrawRect(pos_x, pos_y, BOX_WIDTH - 1, BOX_HEIGHT - 1, olc::VERY_DARK_GREY);
                break;
            case CORRECT_POS:
                FillRect(pos_x, pos_y, BOX_WIDTH, BOX_HEIGHT, olc::DARK_GREEN);
                break;
            case WRONG_POS:
                FillRect(pos_x, pos_y, BOX_WIDTH, BOX_HEIGHT, olc::DARK_YELLOW);
                break;
            default:
                FillRect(pos_x, pos_y, BOX_WIDTH, BOX_HEIGHT, olc::VERY_DARK_GREY);
                break;
        }
        
        /* notes:
            DrawString(pos_x
         + 3, pos_y + 3, input_string[random_val], olc::WHITE); thinking of adding the draw string function here and having an
            array for all the positions on the grid and then using that to determine which letter should appear on each box every frame. Every time
            the enter button is pressed the input string should be cleared but the previous letters on the grid should not depend on what is in the
            input string
         */
        
    }
    
    void DrawButton(int pos_x, int pos_y, string letter, int button_type)
    {
        if(button_type == 1)
            FillRect(pos_x, pos_y, S_BUTTON_WIDTH, S_BUTTON_HEIGHT, olc::DARK_GREY);
        else
            FillRect(pos_x, pos_y, N_BUTTON_WIDTH, N_BUTTON_HEIGHT, olc::DARK_GREY);

        DrawString(pos_x + 2, pos_y + 3, letter, olc::WHITE);
    }
    
    void DrawKeyboard(int pos_x, int pos_y)
    {
        int i = 0;
        int start = pos_x;
        while(i < buttons.size())
        {
            DrawButton(pos_x, pos_y, buttons[i].first, buttons[i].second);

            if(i == 10)
            {
                pos_y += N_BUTTON_HEIGHT + 3;
                pos_x = start + (N_BUTTON_WIDTH/2);
            }
            else if(i == 18)
            {
                pos_y += N_BUTTON_HEIGHT + 3;
                pos_x = start - 8;
            }
                
            pos_x += N_BUTTON_WIDTH + 3;
            
            if(i == 19)
                pos_x += 8;
            i++;
        }
    }
    int scanButton(int x, int y, int button_type)
    {
        int mouse_x = GetMouseX();
        int mouse_y = GetMouseY();
        
        int b_width = (button_type == 0) ? N_BUTTON_WIDTH : S_BUTTON_WIDTH;
        int b_height = (button_type == 0) ? N_BUTTON_HEIGHT : S_BUTTON_HEIGHT;
        
        if(mouse_x >= x & mouse_x < (x + b_width))
        {
            if(mouse_y >= y & mouse_y < (y + b_height))
            {
                if(GetMouse(0).bPressed)
                    return 1;
            }
        }
        return 0;
    }
    
    void scanKeyboard(){
        int i = 0;
        static uint8_t word_counter = 0;
        int pos_x = KEYBOARD_X;
        int pos_y = KEYBOARD_Y;
        
        int start = pos_x;
        bool inWord = true;
        while(i < buttons.size())
        {
            if( scanButton(pos_x, pos_y, buttons[i].second) )
            {
                if(inWord)
                {
                    if (buttons[i].first == "<-")
                    {
                        grid_letters[grid_idx -= 1] = "";
                    }
                    else if (buttons[i].first == "->" && word_counter == 5)
                    {
                        inWord = false; // this assumes that the person doesn't input a word that does not exist
                        word_counter = 0;
                    }
                    else if (word_counter < 5)
                    {
                        grid_letters[grid_idx] = buttons[i].first;
                        grid_idx++;
                        word_counter++;
                    }
                }
                else
                {
                    inWord = true;
                }
                    
            }
            
            if(i == 10)
            {
                pos_y += N_BUTTON_HEIGHT + 3;
                pos_x = start + (N_BUTTON_WIDTH/2);
            }
            else if(i == 18)
            {
                pos_y += N_BUTTON_HEIGHT + 3;
                pos_x = start - 8;
            }
                
            pos_x += N_BUTTON_WIDTH + 3;
            
            if(i == 19)
                pos_x += 8;
            i++;
        }
    }
    
    void isEqual()
    {
        int i = 0;
        for(auto letter : input_word)
        {
            if(letter.first == word[i])
            {
                letter.second = CORRECT_POS;
                box_status[i] = letter.second;
            }
            else if(letter.first != word[i])
            {
                letter.second = WRONG_POS;
                box_status[i] = letter.second;
            }
            i++;
        }
    }
};


int main(int argc, char const *argv[]) {
	Wordle demo;
	if (demo.Construct(360, 250, 3, 3))
		demo.Start();

	return 0;
}
