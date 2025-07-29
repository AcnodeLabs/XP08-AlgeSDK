#ifndef XFUNCTIONS_HPP
#define XFUNCTIONS_HPP
#include "CBaseV1_2.h"

void replaceSubstring(std::string& originalString, const std::string& substringToReplace, const std::string& replacementString) {
	size_t pos = originalString.find(substringToReplace);

	while (pos != std::string::npos) {
		originalString.replace(pos, substringToReplace.length(), replacementString);
		pos = originalString.find(substringToReplace, pos + replacementString.length());
	}
}

float fontGlobalScale = 1.0;
//Very Fuzzy way to fit window in available space
i2 bestfit(i2 availablespace, i2 devicedim) {
	i2 ret = i2(devicedim.x * 1., devicedim.y * 1.); 

	float ar = float(devicedim.y) / float(devicedim.x);
	i2 ret1 = i2(availablespace.x, availablespace.y * ar);
	
	for (int i = 0; i <6; i++) {
		if (ret.y > availablespace.y) ret = ret.mult(0.9);
		if (ret.y < availablespace.y * 0.75 ) break;
	}
	ret.y -= 50;//TASK BAR HEIGHT
	return ret;
}

class XFunction {

};

class XFunction_ReadFileToVectorOfStrings : public XFunction {
public:
	void Load(string filename) {
		
	}
};

class XFunction_AutoScalingToFullScreen : public XFunction {
public:
	static const int AUTO_SCALING_FULLSCREEN = -1;
	static void GameObjectAdded(GameObject* g, float scale) {
		if (scale == AUTO_SCALING_FULLSCREEN) g->scale = scale;
	}

	static void GetDimensionsOf(PosRotScale* that, int *mm_width, int *mm_height, bool portrait) {
		if (that->scale == AUTO_SCALING_FULLSCREEN) {
			*mm_width = that->originalScale;
			*mm_height = that->originalScale / that->originalAspect;
		}
	}
	
};
#endif
