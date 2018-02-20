#pragma once
class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	void virtual init() {};
	void virtual update() {};
	void virtual render() {};

	GameObject2D& getObjectByIndex(int index) const;
	GameObject2D& getObjectByTag(std::string tag) const;

private:
	//This may cause massive issues later...
	std::vector<std::shared_ptr<GameObject2D>> game_objects;
};

