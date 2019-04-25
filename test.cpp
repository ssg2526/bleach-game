#include <iostream>
#include<cmath>
using namespace std;

int main(){
	float tempJvel = 2.0;
	float TIME_STEP = 1.0/60.0;
	float GRAVITY = 10.0;
	int yDelPos = 400;
	float SCALE = 60;
	for(int i=0 ; i<60 ; i++){
		yDelPos += round((-1*tempJvel*TIME_STEP + (0.5)*GRAVITY*TIME_STEP*TIME_STEP)*SCALE);
		tempJvel = tempJvel - (GRAVITY*TIME_STEP);
		cout<<yDelPos<<"   "<<tempJvel<<"   "<<i<<endl;
	}
}