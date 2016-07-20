#pragma once
#include "UniformManager.h"
#include "cinder/gl/Environment.h"
#include "UI.h"

using namespace std;
using namespace ci;
using namespace reza::ui;

class UniformManager {
public:
	void setUniforms(SuperCanvasRef ui, vector<gl::GlslProg::Uniform> uniforms, string source);
	static map<string, string> GetCommentMap(string data);
	map<string, float> fUniforms;
	map<string, int> iUniforms;
	map<string, bool> bUniforms;
	map<string, vec2> v2Uniforms;
	map<string, vec3> v3Uniforms;
	map<string, vec4> v4Uniforms;
	map<string, ivec2> i2Uniforms;
	map<string, ivec3> i3Uniforms;
	map<string, ivec4> i4Uniforms;
	SuperCanvasRef ui;
};