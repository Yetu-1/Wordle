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

struct GRID
{
    string letter = "";
    square_t box_status = DEFAULT;
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

    GRID grid[30];
    uint8_t grid_idx = 0;

    vector<string> word = {"P", "E", "L", "L", "O"};
    
    bool OnUserCreate() override {
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::BLACK);
        scanKeyboard();
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
        
        DrawKeyboard(KEYBOARD_X, KEYBOARD_Y);
        
        DrawGrid(pos_x, pos_y);
    }
    
    void DrawGrid(int pos_x, int pos_y)
    {
        int i = 0;
        int random_val;
        
        while(i < 30)
        {
            random_val = i % 5;
            pos_x += (BOX_WIDTH + 3);
            DrawBox(pos_x, pos_y, grid[i].box_status);
            DrawString(pos_x + 3, pos_y + 3, grid[i].letter, olc::WHITE);
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
                FillRect(pos_x, pos_y, BOX_WIDTH, BOX_HEIGHT, olc::DARK_GREY);
                break;
        }
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
                    if (buttons[i].first == "<-") // ###### notes: pressing backspace mulitple times breaks the code #####
                    {
                        grid[grid_idx -= 1].letter = "";
                        word_counter--;
                    }
                    else if (buttons[i].first == "->" && word_counter == 5)
                    {
                        inWord = false; // this assumes that the person doesn't input a word that does not exist
                        word_counter = 0;
                    }
                    else if (word_counter < 5)
                    {
                        grid[grid_idx].letter = buttons[i].first;
                        grid_idx++;
                        word_counter++;
                    }
                }
                
                if(!inWord)
                {
                    inWord = true;
                    if(isEqual())
                    {
                        grid_idx = 0;
                        memset(grid, 0, sizeof(grid));
                    }
                        
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
    
    bool isEqual()
    {
        int j = 0;
        bool equal_flag = true;
        
        for(int i = (grid_idx - 5); i < (grid_idx); i++)
        {
            if(grid[i].letter == word[j])
            {
                grid[i].box_status = CORRECT_POS;
            }
            else if(grid[i].letter != word[j] && exist(grid[i].letter))
            {
                grid[i].box_status = WRONG_POS;
                equal_flag = false;
            }
            else if(!exist(grid[i].letter))
            {
                grid[i].box_status = WRONG;
                equal_flag = false;
            }
            j++;
        }
        return equal_flag;
    }
    int exist(string letter)
    {
        int j = 0;
        
        for(int i = (grid_idx - 5); i < (grid_idx); i++)
        {
            if(letter == word[j])
            {
               return 1;
            }
            j++;
        }
        return 0;
    }
};


int main(int argc, char const *argv[]) {
	Wordle demo;
	if (demo.Construct(360, 250, 3, 3))
		demo.Start();

	return 0;
}
