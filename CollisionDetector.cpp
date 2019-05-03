#include "CollisionDetector.h"
#include <vector>
#include <math.h>
using namespace std;

CollisionDetector::CollisionDetector(){}

// CollisionDetector::~CollisionDetector(){
// 	
// }

void CollisionDetector::checkCollision(vector<GameObj*> &object){
	for(int i=0; i<object.size(); i++){
		for(int j=0; j<object.size() && j!=i; j++){
			if((*object[i]).name != "ground" || (*object[j]).name != "ground" && j!=i){
				if(abs(((*object[i]).collisionBox.x+(*object[i]).collisionBox.w/2)-((*object[j]).collisionBox.x+(*object[j]).collisionBox.w/2)) < abs((*object[i]).collisionBox.w/2+(*object[j]).collisionBox.w/2)){
					if(abs(((*object[i]).collisionBox.y+(*object[i]).collisionBox.h/2)-((*object[j]).collisionBox.y+(*object[j]).collisionBox.h/2)) < abs((*object[i]).collisionBox.h/2+(*object[j]).collisionBox.h/2)){
						if((*object[i]).name=="e1"){ 
							if((*object[j]).name=="ground"){
								(*object[i]).enemyHitStatic(*object[j]);
							}
						}
						if((*object[i]).name=="player"){
							if((*object[j]).name=="ground"){
								(*object[i]).playerHitStatic(*object[j]);
							}
						}
						if((*object[i]).name=="pbullet"){
							if((*object[j]).name=="e1"){
								(*object[j]).bulletHitEnemy(*object[i]);
							}
						}
						if((*object[i]).name=="ebullet"){
							if((*object[j]).name=="player"){
								(*object[j]).bulletHitPlayer(*object[i]);
							}
						}
					}
				}
			}
			
		}
	}
}
