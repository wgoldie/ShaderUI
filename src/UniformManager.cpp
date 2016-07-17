#include "UniformManager.h"
#include "cinder/gl/Environment.h"
#include "UI.h"
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace ci;
using namespace reza::ui;
using namespace boost;

void UniformManager::setUniforms(SuperCanvasRef ui, vector<gl::GlslProg::Uniform> uniforms)
{
	for (gl::GlslProg::Uniform u : uniforms) {
		string name = u.getName();
		typedef vector< string > split_vector_type;
		split_vector_type tokensplit;
		split(tokensplit, name, is_any_of("_"), token_compress_on);
		std::vector<int> range;
		std::transform(tokensplit.begin() + 1, tokensplit.end(), std::back_inserter(range),
			[](const std::string& str) { return std::stoi(str); });
		string displayName = tokensplit[0];
		int tokens = range.size();
		switch (u.getType()) {
		case GL_FLOAT:
			range = (tokens == 2) ? range : vector<int>{ 0, 1 };
			ui->addSliderf(displayName, &fUniforms[name], range[0], range[1]);
			break;
		case GL_BOOL:
			ui->addToggle(displayName, &bUniforms[name]);
			break;
		case GL_INT:
			range = (tokens == 2) ? range : vector<int>{ 0, 1 };
			if (range == vector<int>{ 0, 1 }) {
				ui->addToggle(displayName, (bool*)&iUniforms[name]);
			}
			else {
				ui->addSlideri(displayName, &iUniforms[name], range[0], range[1]);
			}
			break;
		case GL_FLOAT_VEC2:
			range = (tokens == 4) ? range : vector<int>{ 0, 1, 0, 1 };
			ui->addMultiSlider(displayName, {
				MultiSlider::Data("X", &v3Uniforms[name].x, range[0], range[1]),
				MultiSlider::Data("Y", &v2Uniforms[name].y, range[2], range[3]),
			});
			break;
		case GL_FLOAT_VEC3:
			range = (tokens == 6) ? range : vector<int>{ 0, 1, 0, 1, 0, 1 };
			ui->addMultiSlider(displayName, {
				MultiSlider::Data("X", &v3Uniforms[name].x, range[0], range[1]),
				MultiSlider::Data("Y", &v3Uniforms[name].y, range[2], range[3]),
				MultiSlider::Data("Z", &v3Uniforms[name].z, range[4], range[5])
			});
			break;
		case GL_FLOAT_VEC4:
			range = (tokens == 8) ? range : vector<int>{ 0, 1, 0, 1, 0, 1, 0, 1 };
			ui->addMultiSlider(displayName, {
				MultiSlider::Data("X", &v4Uniforms[name].x, range[0], range[1]),
				MultiSlider::Data("Y", &v4Uniforms[name].y, range[2], range[3]),
				MultiSlider::Data("Z", &v4Uniforms[name].z, range[4], range[5]),
				MultiSlider::Data("W", &v4Uniforms[name].w, range[5], range[6])
			});
			break;
		}
	}

	ui->autoSizeToFitSubviews();
}
