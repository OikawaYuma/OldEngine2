#pragma once
#include <string>
#include <vector>
#include "Transform.h"
#include "Model.h"
#include "Object3d.h"
#include "Camera.h"


class Player;
class Floor;
class LevelData {
public:
	struct ObjectData {
		std::string filename;
		Transform transform;
	};
	std::vector<ObjectData> objects;

private:
	
	

};
class Loder
{
public:
	static void LoadJsonFileBase(const std::string kDefaultBaseDirectory, const std::string fileName,std::vector<Object3d*>& objects,Camera* camera);
	static void LoadJsonFile(const std::string kDefaultBaseDirectory, const std::string fileName,Player *player, Floor* floor);
};


