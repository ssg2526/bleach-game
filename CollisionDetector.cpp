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
					// if((*object[i]).name=="pbullet"){
					// 			cout<<(*object[i]).collisionBox.x<<"		"<<(*object[i]).name<<endl;

					// }
					if(abs(((*object[i]).collisionBox.y+(*object[i]).collisionBox.h/2)-((*object[j]).collisionBox.y+(*object[j]).collisionBox.h/2)) < abs((*object[i]).collisionBox.h/2+(*object[j]).collisionBox.h/2)){
						// cout<<(*object[i]).name<<"		"<<(*object[j]).name<<endl;
						if((*object[i]).name=="e1"){ 
							if((*object[j]).name=="ground"){
								// cout<<(*object[j]).name<<"		"<<(*object[i]).name<<endl;
								(*object[i]).enemyHitStatic(*object[j]);
							}
						}
						if((*object[i]).name=="player"){
							if((*object[j]).name=="ground"){
								// cout<<(*object[j]).name<<"		"<<(*object[i]).name<<endl;
								(*object[i]).playerHitStatic(*object[j]);
							}
						}
						if((*object[i]).name=="pbullet"){
								// cout<<(*object[j]).name<<"		"<<(*object[i]).name<<endl;
							if((*object[j]).name=="e1"){
								// cout<<(*object[j]).name<<"		"<<(*object[i]).name<<endl;
								(*object[i]).bulletHitEnemy(*object[j]);
								delete object[i];
								object.erase(object.begin()+i);
							}
						}
					}
				}
			}
			
		}
	}
}
