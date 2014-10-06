//
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

//
#include "ContactResolver.h"
#include "Core.h"

ContactResolver::ContactResolver(){

	velocityIterations = Core::getInstance()->getVelocityIterations();
	velocityEpsilon = Core::getInstance()->getVelocityEpsilon();
	positionIterations = Core::getInstance()->getPositionIterations();
	positionEpsilon = Core::getInstance()->getPositionEpsilon();

	if(Core::getInstance()->isCalcIterations() == true){
		countVelocIter = 0;
		countPosIter = 0;
	}
	else{
		countVelocIter = NULL;
		countPosIter = NULL;
	}
}

ContactResolver::~ContactResolver(){

	delete velocityIterations;
	delete velocityEpsilon;
	delete positionIterations;
	delete positionEpsilon;
}

void ContactResolver::prepareContacts(Contact *contArray[], unsigned int numCont, float duration){

	Contact* lastContact = contArray + numCont;
	for(Contact* contact=contArray; contact < lastContact; contact++){

	        contact->calcInternData(duration);
	    }
}

void ContactResolver::resolveContacts(Contact *contArray[], unsigned int numCont, float duration){

	if (numCont == 0){
		return;
	}
	prepareContacts(contArray, numCont, duration);

	adjustPos(contArray, numCont, duration);

	adjustVeloc(contArray, numCont, duration);
}

void ContactResolver::adjustVeloc(Contact *contArray[], unsigned int numContacts, float duration){

	glm::vec3 velocChange[2], rotatChange[2];
	glm::vec3 temp;

	//behandle aufprall der stärke nach
	countVelocIter = 0;
	while(countVelocIter < velocityIterations){
		//finde kontakt mit größter geschwindigkeitsänderung
		float velocPrecision = velocityEpsilon;
	    unsigned int index = numContacts;
	    for(unsigned int i = 0; i < numContacts; i++){
	    	if (contArray[i]->getDesiredVelocity() > velocPrecision){
	    		velocPrecision = contArray[i]->getDesiredVelocity();
	            index = i;
	    	}
	    }
	    if (index == numContacts){
	    	break;
	    }
	    //löse obersten kontakt
	    contArray[index]->applyVelocChange(velocChange, rotatChange);

	    //relative schliefgeschwindigkeit mögl. neu berechnen
	    for (unsigned i = 0; i < numContacts; i++){
	    	if (contArray[i]->collBodies[0]){
	    		if (contArray[i]->collBodies[0] == contArray[index]->collBodies[0]){
	    			//kreuzprodukt/vektorprodukt
	    			temp = glm::cross(rotatChange[0], contArray[i]->relatContPos[0]);

	                temp = temp + velocChange[0];

	            	glm::mat3 temp1 = glm::transpose(contArray[i]->contactToWorld);
	            	glm::vec3 temp2= temp1 * temp;
	                contArray[i]->contactVelocity = contArray[i]->contactVelocity + temp2;

	                contArray[i]->calcDesiredVeloc(duration);
	    		}
	            else if (contArray[i]->collBodies[0]==contArray[index]->collBodies[1]){

	            	//kreuzprodukt/vektorprodukt
	            	temp = glm::cross(rotatChange[1], contArray[i]->relatContPos[0]);

	                temp += velocChange[1];

	                glm::mat3 temp3 = glm::transpose(contArray[i]->contactToWorld);
	                glm::vec3 temp4 = temp3 * temp;
	                contArray[i]->contactVelocity = contArray[i]->contactVelocity + temp4;
	                contArray[i]->calcDesiredVeloc(duration);
	            }
	    	}
	        if (contArray[i]->collBodies[1]){

	        	if (contArray[i]->collBodies[1]==contArray[index]->collBodies[0]){

	        		//kreuzprodukt/vektorprodukt
	        		temp = glm::cross(rotatChange[0], contArray[i]->relatContPos[1]);

	                temp = temp + velocChange[0];

	                glm::mat3 temp5 = glm::transpose(contArray[i]->contactToWorld);
	                glm::vec3 temp6 = temp5 * temp;
	                contArray[i]->contactVelocity = contArray[i]->contactVelocity - temp6;
	                contArray[i]->calcDesiredVeloc(duration);
	        	}
	            else if (contArray[i]->collBodies[1] == contArray[index]->collBodies[1]){

	            	//kreuzprodukt/vektorprodukt
	            	temp = glm::cross(rotatChange[1], contArray[i]->relatContPos[1]);

	                temp = temp + velocChange[1];

	                glm::mat3 temp7 = glm::transpose(contArray[i]->contactToWorld);
	                glm::vec3 temp8 = temp7*temp;
	                contArray[i]->contactVelocity = contArray[i]->contactVelocity - temp8;
	                contArray[i]->calcDesiredVeloc(duration);
	            }
	        }
	    }
	    countVelocIter++;
	}
}

void ContactResolver::adjustPos(Contact *contArray[], unsigned int numContacts, float duration){

	unsigned int i,index;
	glm::vec3 velocChange[2], rotatChange[2];
	float rotatAmount[2];
	float posPrecision;
	glm::vec3 temp;

	// TODO: Remove this
	//???
	for(i=0; i<numContacts; i++){
		contArray[i]->calcInternData(duration);
	}
	//behandle penetration der stärke nach
	countPosIter = 0;
	while(countPosIter < positionIterations){
		//finde größte penetration
		posPrecision = positionEpsilon;
	    index = numContacts;
	    for(i=0;i<numContacts;i++) {
	    	if(contArray[i]->penetration > posPrecision){
	    		posPrecision=contArray[i]->penetration;
	    		index=i;
	    	}
	    }
	    if (index == numContacts){
	    	break;
	    }
	    //löse penetration
	    contArray[index]->applyPosChange(velocChange,rotatChange,rotatAmount,posPrecision);//-positionEpsilon);

	    //aktualisiere kontakte
	    for(i = 0; i < numContacts; i++){
	    	if(contArray[i]->collBodies[0] == contArray[index]->collBodies[0]){
	    		//kreuzprodukt/vektorprodukt
	    		temp = glm::cross(rotatChange[0], contArray[i]->relatContPos[0]);

	            temp = temp + velocChange[0];

	            //x*vector.x + y*vector.y + z*vector.z;
	            float temp1 = (temp.x*contArray[i]->contactNormal.x) + (temp.x*contArray[i]->contactNormal.y) + (temp.z*contArray[i]->contactNormal.z);
	            contArray[i]->penetration = contArray[i]->penetration - rotatAmount[0] * temp1;
	    	}
	    	else if(contArray[i]->collBodies[0] == contArray[index]->collBodies[1]){
	    		//kreuzprodukt/vektorprodukt
	    		temp = glm::cross(rotatChange[1], contArray[i]->relatContPos[0]);

	            temp += velocChange[1];

	            //x*vector.x + y*vector.y + z*vector.z;
	            float temp2 = (temp.x*contArray[i]->contactNormal.x) + (temp.y*contArray[i]->contactNormal.y) + (temp.z*contArray[i]->contactNormal.z);
	            contArray[i]->penetration = contArray[i]->penetration - rotatAmount[1] * temp2;
	    	}
	        if(contArray[i]->collBodies[1]){
	        	if(contArray[i]->collBodies[1] == contArray[index]->collBodies[0]){
	        		//kreuzprodukt/vektorprodukt
	        		temp = glm::cross(rotatChange[0], contArray[i]->relatContPos[1]);

	                temp = temp + velocChange[0];

	                //x*vector.x + y*vector.y + z*vector.z;
	                float temp3 = (temp.x*contArray[i]->contactNormal.x) + (temp.y*contArray[i]->contactNormal.y) + (temp.z*contArray[i]->contactNormal.z);
	                contArray[i]->penetration = contArray[i]->penetration + rotatAmount[0] * temp3;
	        	}
	            else if(contArray[i]->collBodies[1] == contArray[index]->collBodies[1]){
	            	//kreuzprodukt/vektorprodukt
	            	temp = glm::cross(rotatChange[1], contArray[i]->relatContPos[1]);

	                temp = temp + velocChange[1];

	                //x*vector.x + y*vector.y + z*vector.z;
	                float temp4 = (temp.x*contArray[i]->contactNormal.x) + (temp.y*contArray[i]->contactNormal.y) + (temp.z*contArray[i]->contactNormal.z);
	                contArray[i]->penetration = contArray[i]->penetration + rotatAmount[1] * temp4;
	            }
	        }
	    }
	    countPosIter++;
	}
}
