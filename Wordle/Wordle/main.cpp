#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <string>
#include <string.h>

#define BOX_WIDTH 12
#define BOX_HEIGHT 12
#define OFFSET 3


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
            DrawString(103, 10, "Wordle", olc::WHITE);
            DrawBoxes();
            FillRect(GetMouseX(), GetMouseY(), 1, 1);
            return true;
        }
        
    void DrawBoxes(){
        std::string arry[6] = {"W", "O", "R", "D", "L", "E"};
        int pos_x = ((ScreenWidth() - (BOX_WIDTH + OFFSET)*6)/2) - 5;
        int pos_y = 30;
        int i = 0;
        while(i < 30)
        {
            pos_x += (BOX_WIDTH + 3);
            DrawRect(pos_x, pos_y, BOX_WIDTH, BOX_HEIGHT, olc::VERY_DARK_GREY);
            //DrawString(pos_x + 3, pos_y + 3, arry[(i % 5)], olc::WHITE);
            i++;
            if(!(i % 5))
            {
                pos_y += BOX_HEIGHT + 3;
                pos_x = ((ScreenWidth() - (BOX_WIDTH + OFFSET)*6)/2) - 5;
            }
        }

    }
};


int main(int argc, char const *argv[]) {
	Wordle demo;
	if (demo.Construct(250, 150, 5, 5))
		demo.Start();

	return 0;
}
