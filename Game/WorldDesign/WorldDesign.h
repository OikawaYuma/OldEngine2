#pragma once
#include "Object3d.h"
#include <list>
class WorldDesign
{
public:
	void Init();
	void Update();
	void Draw();
public: // Getter
	std::list<Object3d*>& GetWorldDesignObject(){ return objects_; }
private:

	std::list<Object3d*> objects_;
};

