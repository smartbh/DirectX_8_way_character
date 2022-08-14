#pragma once

enum class PlayerState
{
	IDLE,
	WALK,
	ROLL,
	HIT
};

class Player : public Character
{
private:
	ObImage*	walk;
	ObImage*	roll;
	PlayerState plState;
	ObRect*		col;
	
	float		rollTime;

public:
	void Idle(ObCircle* boss);
	void Walk(ObCircle* boss);
	void Roll();
	void Hit(ObRect* boss);

	void Input();
	void LookTarget(Vector2 target, ObImage* img);

	ObRect* getCol();
public:
	Player();
	~Player();

	void Update(ObCircle* boss, ObRect* _boss);
	void Render();
};

