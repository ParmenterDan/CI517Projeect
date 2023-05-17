#include "MyEngineSystem.h"

PhysicsObjects :: PhysicsObjects() {
	float x = 0, y = 0, w = 0, h = 0, lastX = 0, lastY = 0;
	float speedX = 0.0, speedY = 2.0, multiplier = 1.0, mass = 1.0, velocity = 0, lastSpeedX = 0, lastSpeedY = 0;
	bool isActive = false, immovable = false;
	std::string side;
};

PhysicsObjects :: ~PhysicsObjects(){

};

bool MyEngineSystem::enablePhysicsObject() {
	return true;
}

bool MyEngineSystem::disablePhysicsObject() {
	return false;
}

void PhysicsObjects::enablePhysicsObject() {
	isActive = true;
}

void PhysicsObjects::disablePhysicsObject() {
	isActive = false;
}

bool PhysicsObjects::setImmovable(bool setter) {
	if (setter == true) {
		return true;
	}
	else if (setter == false) {
		return false;
	}
	else {
		return NULL;
	}

}

bool MyEngineSystem::boxCollisionPhysics(PhysicsObjects object1, PhysicsObjects object2) {
	// Has Collided
	// Object1 is above object2
	if (object1.y + object1.h < object2.y + object2.h) {
		// Object1 colliding from top
		if (object1.y + object1.h > object2.y) {
			if (object1.x >= object2.x && object1.x <= object2.x + object2.w) {
				return true;
			}
			else if (object1.x + object1.w >= object2.x && object1.x + object1.w <= object2.x + object2.w) {
				return true;
			}
			else {
				return false;
			}
		}
	} // Object1 is below Object2
	else if (object1.y > object2.y) {
		// Object1 colliding from bottom
		if (object1.y < object2.y + object2.h) {
			if (object1.x >= object2.x && object1.x <= object2.x + object2.w) {
				return true;
			}
			else if (object1.x + object1.w >= object2.x && object1.x + object1.w <= object2.x + object2.w) {
				return true;
			}
		}
	} // Objects are level
	else if (object1.y == object2.y && object1.y + object1.h == object2.y + object2.h) {
		if (object1.x >= object2.x && object1.x <= object2.x + object2.w) {
			return true;
		}
		else if (object1.x + object1.w >= object2.x && object1.x + object1.w <= object2.x + object2.w) {
			return true;
		}
	}

	// Has not colided
	return false;
}

std::string MyEngineSystem::borderCollideX(PhysicsObjects object, int screenXBorder, int borderOffsetL, int borderOffsetR) {
	if (object.x <= 0 + borderOffsetL) {
		return "Left";
	}
	else if (object.x + object.w >= screenXBorder - borderOffsetR) {
		return "Right";
	}
	else {
		return "None";
	}
}

std::string MyEngineSystem::borderCollideY(PhysicsObjects object, int screenYBorder, int borderOffsetT, int borderOffsetB) {
	if (object.y <= 0 + borderOffsetT) {
		return "Top";
	}
	else if (object.y + object.h >= screenYBorder - borderOffsetB) {
		return "Bottom";
	}
	else {
		return "None";
	}
}

float MyEngineSystem::calcVelocityX(PhysicsObjects object1, PhysicsObjects object2) {
	// If object1 is movable, calculate X velocity, else, return 0
	if (object1.immovable == false) {
		// Objects both moving right
		if (object1.lastSpeedX > 0 && object2.lastSpeedX > 0) {
			// Swap speeds
			return object2.lastSpeedX;
		} // Both objects moving left
		else if (object1.lastSpeedX < 0 && object2.lastSpeedX < 0) {
			// Swap speeds
			return object2.lastSpeedX;
		} // Objects moving towards eachother
		else if ((object1.lastSpeedX > 0 && object2.lastSpeedX < 0) || (object1.lastSpeedX < 0 && object2.lastSpeedX > 0)) {
			// Swap speeds
			return object2.lastSpeedX;
		} // Only object 1 moving
		else if (object1.lastSpeedX != 0 && object2.lastSpeedX == 0) {
			// Swap speeds
			return object2.lastSpeedX;
		} // Only object2 moving
		else if (object1.lastSpeedX == 0 && object2.lastSpeedX != 0) {
			// Swap speeds
			return object2.lastSpeedX;
		} // Neither objects moving
		else {
			return 0;
		}
	}
	else {
		return 0;
	}	
}

float MyEngineSystem::calcVelocityY(PhysicsObjects object1, PhysicsObjects object2) {
	// If object1 is movable, calculate Y velocity, else, return 0
	if (object1.immovable == false) {
		// Both object moving down
		if (object1.lastSpeedY > 0 && object2.lastSpeedY > 0) {
			// Swap speeds
			return object2.lastSpeedY;
		} // Both objects moving up
		else if (object1.lastSpeedY < 0 && object2.lastSpeedY < 0) {
			// Swap speeds
			return object2.lastSpeedY;
		} // Objects moving towards eachother
		else if (object1.lastSpeedY > 0 && object2.lastSpeedY < 0 || object1.lastSpeedY < 0 && object2.lastSpeedY>0) {
			// Swap speeds
			return object2.lastSpeedY;
		}// Only object 1 moving
		else if (object1.lastSpeedY != 0 && object2.lastSpeedY == 0) {
			// Swap speeds
			return object2.lastSpeedY;
		} // Only object 2 moving
		else if (object1.lastSpeedY == 0 && object2.lastSpeedY != 0) {
			// Swap speeds
			return object2.lastSpeedY;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

std::string MyEngineSystem::boxCollisionSide (PhysicsObjects object1, PhysicsObjects object2) {
	/*std::cout << "\n" << std::to_string(object1.y) << "\n";
	std::cout << "\n" << std::to_string(object2.y + object2.h) << "\n";*/
	if (object1.y == object2.y && object1.y + object1.h == object2.y + object2.h) {
		if (object1.x + object1.w >= object2.x && object1.x + object1.w <= object2.x + object2.w) {
			return "Left";
		}
		else if (object1.x <= object2.x + object2.w && object1.x >= object2.x) {
			return "Right";
		}
		else {
			std::cout << "here";
			return "None";
		}
	} 
	else if (object1.y + object1.h > object2.y && object1.y + object1.h < object2.y + object2.h) {
		if (object1.lastY + object1.h <= object2.lastY) {
			if (object1.x <= object2.x && object1.x + object1.w >= object2.x) {
				return "Top";
			}
			else if (object1.x <= object2.x && object1.x + object1.w >= object2.x + object2.w) {
				return "Top";
			}
			else if (object1.x >= object2.x && object1.x <= object2.x + object2.w && object1.x + object1.w >= object2.x && object1.x + object1.w <= object2.x + object2.w) {
				return "Top";
			}
			else if (object1.x + object1.w >= object2.x && object1.x + object1.w <= object2.x + object2.w) {
				return "Top";
			}
			else if (object1.x >= object2.x && object1.x <= object2.x + object2.w) {
				return "Top";
			}
			else if (object1.x <= object2.x + object2.w && object1.x + object1.w >= object2.x + object2.w) {
				return "Side";
			}
			else {
				return "None";
			}
		}
		else if (object1.lastY + object1.h <= object2.y) {
			return "Top";
		}
		else if (object1.x + object1.w >= object2.x && object1.x <= object2.x) {
			if (object1.y <= object2.y + object2.h) {
				return "Left";
			}
			else {
				return "None";
			}

		}
		else if (object1.x <= object2.x + object2.w && object1.x >= object2.x) {
			return "Right";
		}
		else if (object1.x <= object2.x + object2.w && object1.x + object1.w >= object2.x + object2.w) {
			if (object1.y <= object2.y + object2.h) {
				return "Side";
			}
			else {
				std::cout << "here";
				return "None";
			}
		}
		else {
			return "None";
		}
	}
	else if (object1.y <= object2.y + object2.h) {
		if (object1.lastY >= object2.lastY + object2.h) {
			if (object1.x <= object2.x && object1.x + object1.w >= object2.x) {
				if (object1.lastX + object1.w <= object2.x) {
					return "Left";
				}
				else {
					return "Bottom";
				}
			}
			else if (object1.x >= object2.x && object1.x <= object2.x + object2.w && object1.x + object1.w >= object2.x && object1.x + object1.w <= object2.x + object2.w) {
				return "Bottom";
			}
			else if (object1.x >= object2.x && object1.x <= object2.x + object2.w && object1.x + object1.w >= object2.x && object1.x + object1.w <= object2.x + object2.w) {
				return "Top";
			}
			else if (object1.x + object1.w >= object2.x && object1.x + object1.w <= object2.x + object2.w) {
				return "Top";
			}
			else if (object1.x + object1.w >= object2.x && object1.x + object1.w <= object2.x + object2.w) {
				return "Left";
			}
			else if (object1.x >= object2.x && object1.x <= object2.x + object2.w) {
				if (object1.lastX >= object2.x) {
					return "Right";
				}
				else {
					return "Bottom";
				}
			}
			else {
				return "None";
			}
		}
		else if (object1.x + object1.w >= object2.x && object1.x + object1.w <= object2.x + object2.w) {
			return "Left";
		}
		else if (object1.x <= object2.x + object2.w && object1.x >= object2.x) {
			return"Right";
		}
		else {
			return "None";
		}
	}
	else {
		return "None";
	}
	
}

bool MyEngineSystem::disableObject() {
	return false;
}

bool MyEngineSystem::enableObject() {
	return true;
}

// Legacy functions

//void MyEngineSystem::checkOverlap() {
//
//}
//
//std::string MyEngineSystem::boxCollideX(int x, int boxWidth, int screenWidth) {
//	if (x <= 0) {
//		return "Left";
//	}
//	else if (x >= screenWidth - boxWidth) {
//		return "Right";
//	}
//	else {
//		return "None";
//	}
//}
//
//std::string MyEngineSystem::boxCollideY(int y, int boxHeight, int screenHeight) {
//	if (y <= 0) {
//		return "Top";
//	}
//	else if (y >= screenHeight) {
//		return "Bottom";
//	}
//	else {
//		return "None";
//	}
//}
//
//bool MyEngineSystem::boxCollision(int x, int y, int width, int height, int x2, int y2, int width2, int height2) {
//	// has collided
//	// Rect1 is above Rect2
//	if (y + height < y2 + height2) {
//		// Rect1 colliding from top
//		if (y + height > y2) {
//			if (x >= x2 && x <= x2 + width) {
//				return true;
//			}
//			else if (x + width >= x2 && x + width <= x2 + width2) {
//				return true;
//			}
//		}
//	} // Rect1 is below Rect2
//	else if (y > y2) {
//		// Rect1 colliding from bottom
//		if (y < y2 + height2) {
//			if (x >= x2 && x <= x2 + width) {
//				return true;
//			}
//			else if (x + width >= x2 && x + width <= x2 + width2) {
//				return true;
//			} 
//		}
//	}
//
//	// Has not collided
//	return false;
//}
//
//std::string MyEngineSystem::collisionSide(int x, int y, int width, int height, int x2, int y2, int width2, int height2) {
//	// has collided
//	// Rect1 is above Rect2
//	if (y + height < y2 + height2) {
//		// Rect1 colliding from top
//		if (y + height > y2) {
//			if (x >= x2 && x <= x2 + width) {
//				return "top";
//			}
//			else if (x + width >= x2 && x + width <= x2 + width2) {
//				return "top";
//			}
//		}
//	} // Rect1 is below Rect2
//	else if (y > y2) {
//		// Rect1 colliding from bottom
//		if (y < y2 + height2) {
//			if (x >= x2 && x <= x2 + width) {
//				return "bottom";
//			}
//			else if (x + width >= x2 && x + width <= x2 + width2) {
//				return "bottom";
//			}
//		}
//	}
//
//	//return "left";
//	//return "right";
//}
//
//std::string MyEngineSystem::collisionSideComplex(int x, int y, int lastX, int lastY, int width, int height, int x2, int y2, int lastX2, int lastY2, int width2, int height2) {
//	if (lastX + width <= x2) {
//		if (lastY + height <= y2) {
//			std::cout << "Collision top left of 2";
//		}
//		else if (lastY >= y2 + height2) {
//			std::cout << "Collision bottom left of 2";
//		}
//		else {
//			std::cout << "Collision Left of 2";
//		}
//		return "left";
//	}
//	else if (lastX >= x2 + width2) {
//		if (lastY + height <= y2) {
//			std::cout << "Collision top right of 2";
//		}
//		else if (lastY >= y2 + height2) {
//			std::cout << "Collision bottom right of 2";
//		}
//		else {
//			std::cout << "Collision Right of 2";
//		}
//		return "right";
//	}
//	else if (lastY + height <= y2) {
//		std::cout << "Collision Top of 2";
//		return "top";
//	}
//	else if (lastY >= y2 + height2) {
//		std::cout << "Collision from bottom of 2";
//		return "bottom";
//	}
//	else {
//		return "Nothing";
//	}
//
//}
//
//float MyEngineSystem::calcVelocityX(float speedX1, float mass1, float speedX2, float mass2) {
//	//float momentum;
//
//	//// 1 & 2 moving right
//	//if (speedX1 > 0 && speedX2 > 0) {
//	//	momentum = (speedX1 * mass1) + (speedX2 + mass2);
//	//	if (mass1 == mass2) {
//	//		return speedX2;
//	//	}
//	//}
//
//	//return 0;
//
//	return (speedX1 *= -1);
//}
//
//float MyEngineSystem::calcVelocityY(float speedY1, float mass1, float speedY2, float mass2) {
//	//float momentum;
//
//	//// 1 & 2 moving down
//	//if (speedY1 > 0 && speedY2 > 0) {
//	//	momentum = (speedY1 * mass1) + (speedY2 * mass2);
//	//	if (mass1 == mass2) {
//	//		return speedY2;
//	//	}
//	//}
//	//else {
//	//	return 0;
//	//}
//
//	return (speedY1 *= -1);
//}
//
