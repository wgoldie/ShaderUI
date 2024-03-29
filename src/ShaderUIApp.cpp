#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "UI.h"
#include "cinder/gl/gl.h"
#include "UniformManager.h"
#include <windows.h>
#include <io.h>
#include <fcntl.h>

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace reza::ui;

class ShaderUIApp : public App {
public:
	void setup() override;
	void resize() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;
private:
	void setUniforms();
	void setupUI(DataSourceRef data);
	void loadShader(DataSourceRef data);
	map<string, string> comments;

	SuperCanvasRef mUi;
	UniformManager uniformStore;
	DataSourceRef vert;
	bool shouldLoadFile = false;
	bool shouldReloadShader = false;
	fs::path shaderPath;
	vec2 window;
	gl::FboRef	mFbo;
	gl::GlslProgRef mGlsl;
};

void showWin32Console() {
static const WORD MAX_CONSOLE_LINES = 500;
int hConHandle;
long lStdHandle;
CONSOLE_SCREEN_BUFFER_INFO coninfo;
FILE *fp;
// allocate a console for this app
AllocConsole();
// set the screen buffer to be big enough to let us scroll text
GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
coninfo.dwSize.Y = MAX_CONSOLE_LINES;
SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),
coninfo.dwSize);
// redirect unbuffered STDOUT to the console
lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
fp = _fdopen(hConHandle, "w");
*stdout = *fp;
setvbuf(stdout, NULL, _IONBF, 0);
// redirect unbuffered STDIN to the console
lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
fp = _fdopen(hConHandle, "r");
*stdin = *fp;
setvbuf(stdin, NULL, _IONBF, 0);
// redirect unbuffered STDERR to the console
lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
fp = _fdopen(hConHandle, "w");
*stderr = *fp;
setvbuf(stderr, NULL, _IONBF, 0);
// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
// point to console as well
std::ios::sync_with_stdio();
}

void ShaderUIApp::setup()
{
	showWin32Console();
	mFbo = gl::Fbo::create(getWindowWidth(), getWindowHeight());
	vert = loadAsset("shaders/common.vert");
	loadShader(loadAsset("shaders/demo.frag"));
	gl::enableDepthWrite();
	gl::enableDepthRead();
}

void ShaderUIApp::setupUI(DataSourceRef data) {
	mUi = SuperCanvas::create("basic");
	mUi->addButton("Reload shader", &shouldReloadShader);
	mUi->addButton("Load shader", &shouldLoadFile);
	mUi->addSpacer();
	uniformStore = UniformManager();
	uniformStore.setUniforms(mUi, mGlsl->getActiveUniforms(), loadString(data));
}

void ShaderUIApp::resize()
{
	window = getWindowSize();
	mGlsl->uniform("TexDims0", window);
	mFbo = gl::Fbo::create(window.x, window.y);
}

void ShaderUIApp::mouseDown(MouseEvent event)
{
}


void ShaderUIApp::loadShader(DataSourceRef data) {
	try {
		auto newShader = gl::GlslProg::create(vert, data);
		mGlsl = newShader;
		setupUI(data);
	}
	catch (Exception e) {
		cout << e.what();
	}
}

void ShaderUIApp::update()
{
	if (shouldLoadFile) {
		fs::path path = getOpenFilePath("");
		if (fs::exists(path))
		{
			shaderPath = path;
			auto data = loadFile(path);
			loadShader(data);
		}

		shouldLoadFile = false;
	}

	if (shouldReloadShader) {
		if (fs::exists(shaderPath))
		{
			auto data = loadFile(shaderPath);
			loadShader(data);
		}

		shouldReloadShader = false;
	}
}

void ShaderUIApp::setUniforms() {
	auto sh = gl::context()->getGlslProg();
	for (auto const &v : uniformStore.fUniforms) {
		sh->uniform(v.first, v.second);
	}
	for (auto const &v : uniformStore.iUniforms) {
		sh->uniform(v.first, v.second);
	}
	for (auto const &v : uniformStore.v3Uniforms) {
		sh->uniform(v.first, v.second);
	}
	for (auto const &v : uniformStore.i3Uniforms) {
		sh->uniform(v.first, ivec3(v.second));
	}
}

void ShaderUIApp::draw()
{
	gl::clear(Color(0.2f, 0.2f, 0.2f));
	gl::ScopedGlslProg shader(mGlsl);
	setUniforms();
	gl::drawSolidRect(cinder::Rectf(0, 0, window.x, window.y));
}

CINDER_APP(ShaderUIApp, RendererGl)
