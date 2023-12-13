#pragma once

class MouseUtils {

public:
	MouseUtils() = default;
	~MouseUtils() = default;
private:

public:
	Vec3 GetPositionUnderCursor();
	Vec2 GetCursorScreenPosition();
	IEntity* GetEntityUnderCursor();
};

static MouseUtils* g_MouseUtils = new MouseUtils();
