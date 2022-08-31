#pragma once


class game_state
{
public:
	virtual void init() {}
	virtual void update() {}
	virtual void render() {}
	virtual void shutdown() {}
	virtual ~game_state() {}
};

class state_manger
{
public:
	state_manger()
	{
		state = nullptr;
	}

	~state_manger()
	{
		if (state)
			state->shutdown();
		delete this->state;
	}
	void set_state(game_state* state)
	{
		// state may be null so make sure that its vaid
		if (this->state)
			this->state->shutdown();

		// free state memory
		delete this->state;

		// set state
		this->state = state;

		// state may be null so make sure that its vaid
		if (state)
			state->init();
	}

	void update()
	{
		if (state)
			state->update();
	}
	void render()
	{
		if (state)
			state->render();
	}
private:
	game_state* state;
};