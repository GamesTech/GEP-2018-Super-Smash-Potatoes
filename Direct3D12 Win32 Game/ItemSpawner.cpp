#include "pch.h"
#include "ItemSpawner.h"

const std::vector<std::unique_ptr<Item>> &ItemSpawner::getItems() const
{
	return items;
}

void ItemSpawner::update()
{

	//Update items loop
	for (auto& item : items)
	{
		if(item->getMarked())item->SetPos(Vector2(-100, 0));
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
	//Find a spot in the list if there is one
	for (auto& item : items)
	{
		if (item->getMarked())
		{
			item->SetPos(position);
			item->CentreOrigin();
			item->SetScale(Vector2(1, 1));
			item->SetLayer(0.1f);
			item->SetRect(1, 1, 64, 64); //todo
			item->setMarked(false);
			return; //return if found spot
		}
	}

	//Otherwise add new spot
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
	int counter = 0;

	for (auto& item : items)
	{
		if (!item->getMarked())counter++;
	}

	return counter;
}
