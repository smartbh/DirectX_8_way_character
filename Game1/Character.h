#pragma once

enum DirState
{
	Dir_L,
	Dir_R,
	Dir_T,
	Dir_B,
	Dir_LT,
	Dir_LB,
	Dir_RT,
	Dir_RB,
	Dir_N
};

class Character
{
protected:
	ObRect* col;
	Vector2 moveDir;

	int dirState;
	int frameY[8];
};

