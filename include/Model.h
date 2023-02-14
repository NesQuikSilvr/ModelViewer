#include "Skeleton.h"
#include "Skin.h"
#include "Animation.h"

class Model {
public:
	Model();
	~Model();

	Skeleton* skel;
	Skin* skin;
	Animation* anim;
};