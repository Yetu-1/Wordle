#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <string>
#include <string.h>

#define BOX_WIDTH 13
#define BOX_HEIGHT 13
#define OFFSET 3

enum square_t
{
    DEFAULT,
    CORRECT_POS,
    WRONG_POS,
    WRONG
};

class Wordle : public olc::PixelGameEngine {
    public:
        Wordle() {
            sAppName = "Wordle";
        }

    public:
        bool OnUserCreate() override {
            // Called once at the start, so create things here
            return true;
        }

        bool OnUserUpdate(float fElapsedTime) override {
            Clear(olc::BLACK);
            DrawFrame();
            FillRect(GetMouseX(), GetMouseY(), 1, 1);
            return true;
        }
        
    void DrawFrame()
    {
        DrawString(103, 10, "Wordle", olc::WHITE);
        DrawLine(0, 22, ScreenWidth(), 22, olc::DARK_GREY);

        std::string arry[6] = {"W", "O", "R", "D", "L", "E"};
        int pos_x = ((ScreenWidth() - (BOX_WIDTH + OFFSET)*6)/2) - 5;
        int pos_y = 33;
        int i = 0;
        square_t b_type = DEFAULT;
        int random_val;
        while(i < 30)
        {
            random_val = i % 5;
            if(arry[random_val] == "W" || arry[random_val] == "R")
                b_type = CORRECT_POS;
            else
                b_type = DEFAULT;
            
            pos_x += (BOX_WIDTH + 3);
            DrawBox(pos_x, pos_y, b_type);
            DrawString(pos_x + 3, pos_y + 3, arry[random_val], olc::WHITE);
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
        
    }
};


int main(int argc, char const *argv[]) {
	Wordle demo;
	if (demo.Construct(250, 150, 5, 5))
		demo.Start();

	return 0;
}
