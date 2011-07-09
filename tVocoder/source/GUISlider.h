#ifndef _GUI_SLIDER
#define _GUI_SLIDER

using namespace std;

class GUISlider{
	public:
		GUISlider();
		GUISlider(float x, float y, float width, float height, float minVal, float maxVal, float initialVal);
		void setCoords(float x, float y, float width, float height, float minVal, float maxVal, float initialVal);
		void handleClick(int xCoord, int yCoord);
		void setClicked(int x, int y, bool clicked);
		void drawSlider();
		float getValue();
	private:
		float x,y,width, height, minVal,maxVal,val;
		float sliderWidth;
		bool clicked;
};

#endif