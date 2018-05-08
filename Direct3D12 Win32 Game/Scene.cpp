#include "pch.h"
#include "Scene.h"

GameObject2D & Scene::getObjectByIndex(int index) const
{
	return *game_objects.at(index);
}

GameObject2D & Scene::getObjectByTag(std::string tag) const
{
	for (auto& obj : game_objects)
	{
		if (obj->GetTag() == tag)
		{
			return *obj;
		}
	}
}

void Scene::loadBackground(RenderData* m_RD, std::shared_ptr<ImageBuffer> image_buffer)
{
	std::random_device rd;
	std::uniform_real_distribution<float> background(0, 4);
	int i = background(rd);
	background_file = std::make_unique<LevelFile>();
	background_file->read("backgrounds", ".objs");

	string temp_name = background_file->getObj(i).image_file;
	game_objects.emplace_back(new ImageGO2D(m_RD, temp_name, image_buffer));
	game_objects.back()->SetPos(background_file->getObj(i).position);
	game_objects.back()->SetOrigin(background_file->getObj(i).origin);
	game_objects.back()->SetScale(background_file->getObj(i).scale);
	game_objects.back()->SetOri(background_file->getObj(i).orientation);
	game_objects.back()->SetLayer(background_file->getObj(i).layer);
	game_objects.back()->SetType(background_file->getObj(i).type);
	game_objects.back()->SetRect(background_file->getObj(i).sprite_size_min.x, background_file->getObj(i).sprite_size_min.y, background_file->getObj(i).sprite_size_max.x, background_file->getObj(i).sprite_size_max.y);

}
