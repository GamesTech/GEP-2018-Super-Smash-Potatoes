#include "pch.h"
#include "ItemSpawner.h"

const std::vector<std::unique_ptr<Item>> &ItemSpawner::getItems() const
{
	return items;
}

void ItemSpawner::update()
{
	//Delete used items
	for (int i = 0; i < items.size(); i++) {
		if (items[i]->getMarked()) {
			items.erase(items.begin() + i);
		}
	}

	//Update items loop
	for (auto& item : items)
	{
		item->update();
	}
}

void ItemSpawner::render(RenderData* _RD)
{
	for (auto& item : items)
	{
		item->Render(_RD);
	}
}

void ItemSpawner::addItem(Vector2 position, RenderData * _RD, string _filename, Item::Type type)
{
	auto item = new Item(_RD, _filename, type);
	item->SetPos(position);
	item->CentreOrigin();
	item->SetScale(Vector2(1, 1));
	item->SetLayer(0.1f);
	item->SetRect(1, 1, 64, 64); //todo
	items.emplace_back(item);
}

int ItemSpawner::getSize() const
{
	return items.size();
}
