#pragma once
#include <vector>
#include <memory>
#include "Item.h"

class ItemSpawner {
	public:
		ItemSpawner();
		~ItemSpawner() = default;

		const std::vector<std::unique_ptr<Item>>& getItems() const;
		void init(RenderData* _RD, std::shared_ptr<ImageBuffer> image_buffer, std::shared_ptr<ParticleSystem> particle_system);
		void update(GameStateData * _GSD);
		void render(RenderData* _RD);
		void addItem(Vector2 position, string _filename, Item::Type type, long life_span);
		int getSize() const;
	private:
		std::vector<std::unique_ptr<Item>> items;
		RenderData* m_RD = nullptr;
		std::shared_ptr<ImageBuffer> image_buffer = nullptr;
		std::shared_ptr<ParticleSystem> particle_system = nullptr;
};