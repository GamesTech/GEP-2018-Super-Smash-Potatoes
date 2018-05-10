#pragma once
#include "ImageGO2D.h"

class Item : public Physics2D
{
	public:
		enum Type {
			BOMB,
			HEAL,
		};

		Item(RenderData* _RD, string _filename, std::shared_ptr<ImageBuffer> image_buffer, Type type, long life_span);

		void collided(class Player2D* player, GameStateData * _GSD);
		void update(GameStateData * _GSD);
		void setActive(bool active);
		void setMarked(bool marked);
		void setLife(long life);
		void throwItem(GameStateData* _GSD, float player_orientation);
		bool getMarked() const;
	private:
		static const int yOffset = 100;

		Type type;
		class Player2D * player = nullptr;
		int prev_holder_id = -1;
		bool active = false;
		bool marked_for_deletion = false;
		long life_span;
		long time = 0;
};