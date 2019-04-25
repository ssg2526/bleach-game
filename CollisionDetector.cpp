#include "CollisionDetector.h"
#include <vector>
#include <math.h>
using namespace std;

CollisionDetector::CollisionDetector(){}

// CollisionDetector::~CollisionDetector(){
// 	
// }

void CollisionDetector::checkCollision(vector<GameObj*> &object){
	for(int i=0; i<3/*object.size()*/; i++){
		for(int j=i+1; j<4/*object.size()*/; j++){
			if(abs(((*object[i]).collisionBox.x+(*object[i]).collisionBox.w/2)-((*object[j]).collisionBox.x+(*object[j]).collisionBox.w/2)) < abs((*object[i]).collisionBox.w/2+(*object[j]).collisionBox.w/2)){
				if(abs(((*object[i]).collisionBox.y+(*object[i]).collisionBox.h/2)-((*object[j]).collisionBox.y+(*object[j]).collisionBox.h/2)) < abs((*object[i]).collisionBox.h/2+(*object[j]).collisionBox.h/2)){
					// cout<<(*object[i]).name<<endl;
					//Player* p = dynamic_cast<Player*>(object[i]);
					if((*object[i]).name=="e1"){ 
						if((*object[j]).name=="game_object"){
							// cout<<(*object[j]).name<<"		"<<(*object[i]).name<<endl;
							(*object[i]).enemyHitStatic(*object[j]);
						}
					}
					if((*object[i]).name=="player"){
						if((*object[j]).name=="game_object"){
							(*object[i]).playerHitStatic(*object[j]);
						}
					}
					// if((*object[j]).name == "game_object"){
					// 	//cout<<"aaaa";
					// 	if((*p).name == "player"){
					// 		(*p).playerHitStatic(*object[j]);
					// 	}
					// }

				}
			}
		}
	}
}
