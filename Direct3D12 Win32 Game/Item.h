#pragma once
#include "ImageGO2D.h"

class Item : public ImageGO2D
{
	public:
		enum Type {
			BOMB
		};

		Item(RenderData* _RD, string _filename, Type type);


		void collided(Player2D* player, GameStateData * _GSD);
		void update();
	private:
		Type type;
		boolean active = true;
};