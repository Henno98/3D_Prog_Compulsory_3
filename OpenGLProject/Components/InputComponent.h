#pragma once
class InputComponent
{
	bool CanMove = false;
public:
	InputComponent(bool state) : CanMove(state)
	{
	}
	bool GetState()
	{
		return CanMove;

	}
	void SetState(bool newstate)
	{
		CanMove = newstate;

	}
};

