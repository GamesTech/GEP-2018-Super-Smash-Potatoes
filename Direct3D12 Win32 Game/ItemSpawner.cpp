#include "pch.h"
#include "ItemSpawner.h"

const std::vector<std::unique_ptr<Item>> &ItemSpawner::getItems() const
{
	return items;
}

void ItemSpawner::init(RenderData * _RD, std::shared_ptr<ImageBuffer> _image_buffer, std::shared_ptr<ParticleSystem> _particle_system)
{
	m_RD = _RD;
	image_buffer = _image_buffer;
	particle_system = _particle_system;
}

void ItemSpawner::update(GameStateData * _GSD)
{

	//Update items loop
	for (auto& item : items)
	{
		if(item->getMarked()) item->SetPos(Vector2(-100000, 0));		//set offscreen
		item->update(_GSD);
	}
}

void ItemSpawner::render(RenderData* _RD)
{
	for (auto& item : items)
	{
		item->Render(_RD);
	}
}

void ItemSpawner::addItem(Vector2 position, string _filename, Item::Type type, long life_span)
{
	//Find a spot in the list if there is one
	for (auto& item : items)
	{
		if (item->getMarked())
		{
			item->SetPos(position);
			item->CentreOrigin();
			item->SetScale(Vector2(1, 1));
			item->SetLayer(0.5f);
			item->SetRect(1, 1, 53, 69); //todo
			item->setMarked(false);
			item->setActive(false);
			item->setLife(life_span);
		
			return; //return if found spot
		}
	}

	//Otherwise add new spot
	items.emplace_back(new Item(m_RD, _filename, image_buffer, type, life_span, particle_system));
	items.back()->SetPos(position);
	items.back()->CentreOrigin();
	items.back()->SetScale(Vector2(1, 1));
	items.back()->SetLayer(0.5f);
	items.back()->SetRect(1, 1, 54, 69); //todo
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
