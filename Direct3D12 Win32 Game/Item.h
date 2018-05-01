#pragma once
#include "ImageGO2D.h"

class Item : public Physics2D
{
	public:
		enum Type {
			BOMB
		};

		Item(RenderData* _RD, string _filename, Type type);


		void collided(class Player2D* player, GameStateData * _GSD);
		void update();
		void resetPlayer();
		void setActive(bool active);
		void setMarked(bool marked);
		bool getMarked() const;
	private:
		static const int yOffset = 50;

		Type type;
		class Player2D * player = nullptr;
		bool active = true;
		bool marked_for_deletion = false;
};