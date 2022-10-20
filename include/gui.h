#ifndef IGUI_H
#define IGUI_H

#include <vector>

class IGui
{
public:
	virtual void render() = 0;
};

class CameraControlsUI : public IGui
{
public:
	CameraControlsUI(float& yaw, float& pitch, float& radius, bool& ortho);
	virtual void render();

private:
	float& _yaw;
	float& _pitch;
	float& _radius;
	bool& _ortho;
};

struct WorldPosition
{
	float x, y, z;
};

class LightingControlsUI : virtual public IGui
{
public:
	LightingControlsUI(WorldPosition light_pos, bool& auto_movement);
	virtual void render();

private:
	WorldPosition _light_pos;
	bool& _auto_movement;
};

class RenderOptionsUI : virtual public IGui
{
public:
	RenderOptionsUI(bool& wireframe);
	virtual void render();

private:
	bool& _wireframe;
};

#endif 