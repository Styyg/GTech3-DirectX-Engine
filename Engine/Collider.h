#pragma once

struct coordinate {
	int x;
	int y;
	int z;
};

class Collider
{
	struct coordinate {
		int x;
		int y;
		int z;
	};
public:
	Collider();
	~Collider();
private:
	int mSize;
	int mCenter;
};

