#pragma once
#include <vector>
#include <memory>
#include "Item.h"

class ItemSpawner {
	public:
		ItemSpawner() = default;
		~ItemSpawner() = default;

		const std::vector<std::unique_ptr<Item>>& getItems() const;
		void update();
		void render(RenderData* _RD);
		void addItem(Vector2 position, RenderData * _RD, string _filename, Item::Type type);

		int getSize() const;
	private:
		std::vector<std::unique_ptr<Item>> items;
};