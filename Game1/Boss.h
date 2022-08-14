#pragma once

enum class BossState
{
	IDLE,
	WALK,
	ATTACK
};

class Boss : public Character
{
private:
	ObImage*	idle;
	BossState	bossState;

	ObCircle*	detect;
	ObCircle*	follow;
	ObCircle*	attack;

	ObRect*		col;

	Vector2		bossDir;
	float		attackTime;
public:
	void		Idle(ObRect* _player);
	void		Walk(ObRect* _player);
	void		attackStance(ObRect* _player);

	void		LookTarget(Vector2 target, ObImage* img);
	ObRect*		getCol();
	ObCircle*	getAttackCircle();

public:
	Boss();
	~Boss();

	void		Update(ObRect* _player);
	void		Render();
};

