#include "UniformManager.h"
#include "cinder/gl/Environment.h"
#include "UI.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include "cinder/Xml.h"

using namespace std;
using namespace ci;
using namespace reza::ui;
using namespace boost;


map<string, string> UniformManager::GetCommentMap(string data) {
	map<string, string> r;
	vector<string> lines;
	split(lines, data, is_any_of("\n\r"), token_compress_on);
	for (string line : lines)
	{
		vector<string> tokens;
		split(tokens, line, is_any_of(" \t"), token_compress_on);
		vector<string> types = { string("float") };
		auto type = search(tokens.begin(), tokens.end(), types.begin(), types.end());

		if (type == tokens.end()) {
			continue;
		}

		cout << *type;
		auto var = type+1;

		if (var == tokens.end()) {
			cout << "WTF\n";
			continue;
		}

		cout << " " << *(var) << "\n";
		
		vector<string> commentSegments;
		algorithm::split_regex(commentSegments, line, regex("[(\/\*)(//)]"));

		if (commentSegments.size() < 2) {
			continue;
		}

		r[*var] = commentSegments[1];
		cout << commentSegments[1] << "\n";
	}
	
	return r;

}

void UniformManager::setUniforms(SuperCanvasRef ui, vector<gl::GlslProg::Uniform> uniforms, string source)
{
	GetCommentMap(source);
	auto comments = map<string, string>();

	for (gl::GlslProg::Uniform u : uniforms) {
		string name = u.getName();
		XmlTree xml;
		vector<string> rangeS;
		auto comment = comments.find(name);
		if (comment != comments.end()) {
			xml = XmlTree(comment->second);
			if (xml.hasChild("range")) {
				split(rangeS, xml.getChild("range").getValue(), is_any_of(","), token_compress_on);
			}
			else {
				rangeS = vector<string>();
			}
		}

		std::vector<int> range;
		int tokens = rangeS.size();
		if (tokens > 0) {
			std::transform(rangeS.begin() + 1, rangeS.end(), std::back_inserter(range),
				[](const std::string& str) { return std::stoi(str); });
		}

		switch (u.getType()) {
		case GL_FLOAT:
			range = (tokens == 2) ? range : vector<int>{ 0, 1 };
			ui->addSliderf(name, &fUniforms[name], range[0], range[1]);
			break;
		case GL_BOOL:
			ui->addToggle(name, &bUniforms[name]);
			break;
		case GL_INT:
			range = (tokens == 2) ? range : vector<int>{ 0, 1 };
			if (range == vector<int>{ 0, 1 }) {
				ui->addToggle(name, (bool*)&iUniforms[name]);
			}
			else {
				ui->addSlideri(name, &iUniforms[name], range[0], range[1]);
			}
			break;
		case GL_FLOAT_VEC2:
			range = (tokens == 4) ? range : vector<int>{ 0, 1, 0, 1 };
			ui->addMultiSlider(name, {
				MultiSlider::Data("X", &v3Uniforms[name].x, range[0], range[1]),
				MultiSlider::Data("Y", &v2Uniforms[name].y, range[2], range[3]),
			});
			break;
		case GL_FLOAT_VEC3:
			range = (tokens == 6) ? range : vector<int>{ 0, 1, 0, 1, 0, 1 };
			ui->addMultiSlider(name, {
				MultiSlider::Data("X", &v3Uniforms[name].x, range[0], range[1]),
				MultiSlider::Data("Y", &v3Uniforms[name].y, range[2], range[3]),
				MultiSlider::Data("Z", &v3Uniforms[name].z, range[4], range[5])
			});
			break;
		case GL_FLOAT_VEC4:
			range = (tokens == 8) ? range : vector<int>{ 0, 1, 0, 1, 0, 1, 0, 1 };
			ui->addMultiSlider(name, {
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
