#pragma once
#include "GUIScreen.h"

class Texture;
class GUIScrollBar : public GUIScreen
{
public:
	GUIScrollBar();
	virtual ~GUIScrollBar();

public:
	virtual void			Initialize();
	virtual void			Render(const Vector2Int& cameraOffset);
	virtual void			Update(float deltaTime);

protected:
	virtual void			DrawTexture(Texture* texture, const Vector2Int& position, const Vector2Int& cameraOffset = Vector2Int{ 0, 0 }, float scale = 1.f);

public:
	virtual bool			IsInRange(const Vector2Int& position) override;
	virtual void			OnMouseWheel(const Vector2Int& position) override;

public:
	int32					GetValue() { return _value; }
	void					SetValue(int32 value) { _value = value; }

	void					SetMaxValue();
	float					GetRation();

private:
	Texture*				_sliderTexture = nullptr;
	int32					_value = 0;

	const int32				_MaxValue = 5;
	const int32				_MinValue = -180;
};

