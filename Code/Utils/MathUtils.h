#pragma once

class MathUtils {
public:
	int32 GetRandomInt(int32 min, int32 max);
	f32 GetRandomFloat(f32 min, f32 max);
};
static MathUtils* g_MathUtils = new MathUtils();