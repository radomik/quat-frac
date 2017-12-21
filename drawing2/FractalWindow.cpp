#include "stdafx.h"
#include "FractalWindow.hpp"
#include "FractalWindowLoader.hpp"
#include "ProgramArgs.hpp"
#include "OptAppDrawingParser.hpp"
#include "gfx.hpp"
#include <signal.h>
#include "GeomMath.hpp"

FractalWindow*  FractalWindow::self = NULL;
const GLenum	FractalWindow::LIGHT_IDS[] = {GL_LIGHT0, GL_LIGHT1, GL_LIGHT2};

#define _st_setGlOpt(opt, enabled, out_val) 									\
{																				\
	if (enabled) { glEnable(opt); }												\
	else { glDisable(opt); }													\
	out_val = enabled;															\
	fprintf(stderr, "Set %s %s\n", #opt, out_val ? "enabled" : "disabled");		\
	gfx::setRefresh();															\
}

int FractalWindow::createDrawing(
		FILE*						fractal_data_file, 
		const OptGenerator&			opt_generator, 
		const OptDrawingCreator&	opt_drawing) 
{
	int ret = m_drawing.create(fractal_data_file, opt_generator, opt_drawing);
	if (ret == 0) m_opt_drawing.apply();
	return ret;
}

void FractalWindow::_toggleCameraRotate() {
	if (!m_camera.opt().rotate.enabled_mouse_rotate) return;
	if (!m_drawing.isCreated()) { m_paused = false; return; }

	m_paused = !m_paused;
}

void FractalWindow::_rotate(unsigned int coord_ind, bool value_up) {
	float   t = m_opt_drawing.rotation[coord_ind];

	if (value_up) {
		t += m_opt_drawing.rotate_angle;

		if (Float::cmp(t, 360.0f) > 0) // t > 360 degress
			t -= 360.0f;
	}
	else {
		t -= m_opt_drawing.rotate_angle;

		if (Float::cmp(t, 0.0f) < 0) // t < 0 degress
			t = 360.0f + t;
	}

	m_opt_drawing.rotation[coord_ind] = t;
	gfx::setRefresh();
}

int FractalWindow::_st_readPixels(
		std::vector<unsigned char>& pixel_data, 
		unsigned int				&bytes_per_line,
		unsigned int 				w,
		unsigned int 				h) 
{
	GLenum          format = GL_RGBA;
	GLenum          type = GL_UNSIGNED_BYTE;
	GLenum          errCode;
	unsigned char   components = 4;
	size_t          alloc_size = w * h * components;
	char			size_str[64];
	
	bytes_per_line = w * components;
	
	formatBytes(alloc_size, size_str, sizeof(size_str));
	
		/*fprintf(stderr, "%s: Saving fractal to image file:\n\
 * (img_w,img_h):     (%u,%u)\n\
 * format:            %u (%d components)\n\
 * type:              %u\n\
 * alloc size:        %s\n\
 * bytes_per_line:    %u\n",
		__FUNCTION__, w, h, format, components, 
		type, size_str, bytes_per_line);*/
	
	try {
		pixel_data.resize(alloc_size);
	}
	catch (const std::bad_alloc &ex) {
		fprintf(stderr, "%s: Failed to allocate data of size %s [%s]\n", 
			__FUNCTION__, size_str, ex.what());
		return -1;
	}
	
	//fprintf(stderr, "%s: Reading pixels from window\n", __FUNCTION__);
	glReadPixels(0, 0, w, h, format, type, pixel_data.data());

	if ((errCode = glGetError()) != GL_NO_ERROR) {
		fprintf(stderr, "%s: [ERROR] glReadPixels() failed [%s]\n", 
			__FUNCTION__, gluErrorString(errCode));
		return -1;
	}

	return 0;
}

int FractalWindow::_st_revertPixelData(
	std::vector<unsigned char>& 		pixel_data_rvt,
	const std::vector<unsigned char>&   pixel_data,
	unsigned int						bytes_per_line,
	unsigned int						h)
{
	try {
		pixel_data_rvt.resize(pixel_data.size());
	}
	catch (const std::bad_alloc &ex) {
		char			size_str[64];
		formatBytes(pixel_data.size(), size_str, sizeof(size_str));
		
		fprintf(stderr, "%s: Failed to allocate data of size %s [%s]\n", 
			__FUNCTION__, size_str, ex.what());
		return -1;
	}
	
	const unsigned char *p_in = pixel_data.data() + (h-1)*bytes_per_line;
	unsigned char *p_out = pixel_data_rvt.data();
	
	for (unsigned int i = 0; i < h; i++, p_in -= bytes_per_line, p_out += bytes_per_line) {
		memcpy(p_out, p_in, bytes_per_line);
	}
	
	return 0;
}

int FractalWindow::_st_exportPng() 
{
	std::vector<unsigned char>  pixel_data;
	unsigned int 				w = gfx::windowWidth();
	unsigned int 				h = gfx::windowHeight();
	unsigned int				bytes_per_line;
	
	// Read pixels from window
	//fprintf(stderr, "%s: Reading pixel data\n", __FUNCTION__);
	if (_st_readPixels(pixel_data, bytes_per_line, w, h) < 0) {
		fprintf(stderr, "%s: [ERROR] Failed to read pixel data\n", __FUNCTION__);
		return -1;
	}
	
	// Revert lines of pixel_data
	std::vector<unsigned char>  pixel_data_rvt;
	if (_st_revertPixelData(pixel_data_rvt, pixel_data, bytes_per_line, h) < 0) {
		fprintf(stderr, "%s: [ERROR] Failed to revert pixel data\n", __FUNCTION__);
		return -1;
	}

	if (FractalWindowLoader::get()->exportPng(pixel_data_rvt, w, h) < 0) {
		fprintf(stderr, "%s: Failed to save PNG file\n", __FUNCTION__);
		return -1;
	}

	return 0;
}

inline bool FractalWindow::_st_ctrl_pressed() {
	return (glutGetModifiers() | GLUT_ACTIVE_CTRL) != 0;
}

inline char FractalWindow::_lightOn() {
	return (m_light[m_curr_light].enabled) ? '+' : '-';
}

inline char FractalWindow::_lightOn(unsigned int ind) {
	return (m_light[ind].enabled) ? '+' : '-';
}

void FractalWindow::_showLightPosInfo() {
	sys_snprintf(m_buf, sizeof(m_buf), "(%c)Light %d pos (%.1f,%.1f,%.1f,%.1f)", 
		_lightOn(), m_curr_light+1, m_light[m_curr_light].pos[0], m_light[m_curr_light].pos[1], 
		m_light[m_curr_light].pos[2], m_light[m_curr_light].pos[3]);
	gfx::setWindowTitle(m_buf);
}

void FractalWindow::_showLightDiffuseInfo() {
	sys_snprintf(m_buf, sizeof(m_buf), "(%c)Light %d diffuse (%.1f,%.1f,%.1f,%.1f)", 
		_lightOn(), m_curr_light+1, m_light[m_curr_light].diffuse[0], m_light[m_curr_light].diffuse[1], 
		m_light[m_curr_light].diffuse[2], m_light[m_curr_light].diffuse[3]);
	gfx::setWindowTitle(m_buf);
}

void FractalWindow::_showLightSpotDir() {
	sys_snprintf(m_buf, sizeof(m_buf), "(%c)Light %d spot dir (%.1f,%.1f,%.1f)", 
		_lightOn(), m_curr_light+1, m_light[m_curr_light].spot_dir[0], m_light[m_curr_light].spot_dir[1], 
		m_light[m_curr_light].spot_dir[2]);
	gfx::setWindowTitle(m_buf);
}

void FractalWindow::_showLightSpotCutOff() {
	sys_snprintf(m_buf, sizeof(m_buf), "(%c)Light %d spot cut off: %.1f", 
		_lightOn(), m_curr_light+1, m_light[m_curr_light].spot_cut_off);
	gfx::setWindowTitle(m_buf);
}

void FractalWindow::_showLightSpotExponent() {
	sys_snprintf(m_buf, sizeof(m_buf), "(%c)Light %d spot exponent: %.1f", 
		_lightOn(), m_curr_light+1, m_light[m_curr_light].spot_exponent);
	gfx::setWindowTitle(m_buf);
}

void FractalWindow::_showSelectedLight() {
	sys_snprintf(m_buf, sizeof(m_buf), "(%c)Light %d selected, enabled: %s", 
		_lightOn(), m_curr_light+1, syesno(m_light[m_curr_light].enabled));
	gfx::setWindowTitle(m_buf);
}

void FractalWindow::_showLightState() {
	sys_snprintf(m_buf, sizeof(m_buf), "Lights: (1 %c) (2 %c) (3 %c)", 
		_lightOn(0), _lightOn(1), _lightOn(2));
	gfx::setWindowTitle(m_buf);
}

void FractalWindow::_toggleLight(unsigned int ind) {
	_st_setGlOpt(LIGHT_IDS[ind], !m_light[ind].enabled, m_light[ind].enabled);
	_showLightState();
}

#define LIGHT_STEP 0.5f

void FractalWindow::_changeLightPos(unsigned int coord_ind, bool wheel_up) {
	if (wheel_up)
		m_light[m_curr_light].pos[coord_ind] += LIGHT_STEP;
	else
		m_light[m_curr_light].pos[coord_ind] -= LIGHT_STEP;
	_showLightPosInfo();
}

void FractalWindow::_changeLightDiffuse(unsigned int coord_ind, bool wheel_up) {
	if (wheel_up)
		m_light[m_curr_light].diffuse[coord_ind] += LIGHT_STEP;
	else
		m_light[m_curr_light].diffuse[coord_ind] -= LIGHT_STEP;
	_showLightDiffuseInfo();
}

void FractalWindow::_changeLightSpotDir(unsigned int coord_ind, bool wheel_up) {
	if (wheel_up)
		m_light[m_curr_light].spot_dir[coord_ind] += LIGHT_STEP;
	else
		m_light[m_curr_light].spot_dir[coord_ind] -= LIGHT_STEP;
	_showLightSpotDir();
}

void FractalWindow::_changeLightSpotCutOff(bool wheel_up) {
	if (wheel_up)
		m_light[m_curr_light].spot_cut_off += LIGHT_STEP;
	else
		m_light[m_curr_light].spot_cut_off -= LIGHT_STEP;
	_showLightSpotCutOff();
}

void FractalWindow::_changeLightExponent(bool wheel_up) {
	if (wheel_up)
		m_light[m_curr_light].spot_exponent += LIGHT_STEP;
	else
		m_light[m_curr_light].spot_exponent -= LIGHT_STEP;
	_showLightSpotExponent();
}

void FractalWindow::_mouseWheel(bool wheel_up) {
	if (!m_drawing.isCreated()) return;

	bool z_press = m_keystate['z'] || m_keystate['Z'];
	bool x_press = m_keystate['x'] || m_keystate['X'];
	bool c_press = m_keystate['c'] || m_keystate['C'];

	bool rotate = z_press || x_press || c_press;

	//fprintf(stderr, "%s: z: %s, x: %s, c: %s\n",
	//  __FUNCTION__, sbool(z_press), sbool(x_press), sbool(c_press));

	/// Key Z pressed rotate around X axis
	if (z_press) _rotate(0, wheel_up);

	/// Key X pressed rotate around Y axis
	if (x_press) _rotate(1, wheel_up);

	/// Key C pressed rotate around Z axis
	if (c_press) _rotate(2, wheel_up);
	
	if (m_keystate['u']) {
		if (m_keystate['7']) { _changeLightPos(0, wheel_up); }
		if (m_keystate['8']) { _changeLightPos(1, wheel_up); }
		if (m_keystate['9']) { _changeLightPos(2, wheel_up); }
		if (m_keystate['0']) { _changeLightPos(3, wheel_up); }
		return;
	}
	
	if (m_keystate['i']) {
		if (m_keystate['7']) { _changeLightDiffuse(0, wheel_up); }
		if (m_keystate['8']) { _changeLightDiffuse(1, wheel_up); }
		if (m_keystate['9']) { _changeLightDiffuse(2, wheel_up); }
		if (m_keystate['0']) { _changeLightDiffuse(3, wheel_up); }
		return;
	}
	
	if (m_keystate['o'] != 0) {
		if (m_keystate['7']) { _changeLightSpotDir(0, wheel_up); }
		if (m_keystate['8']) { _changeLightSpotDir(1, wheel_up); }
		if (m_keystate['9']) { _changeLightSpotDir(2, wheel_up); }
		return;
	}
	
	if (m_keystate['j']) {
		_changeLightSpotCutOff(wheel_up);
		return;
	}
	
	if (m_keystate['k']) {
		_changeLightExponent(wheel_up);
		return;
	}

	/// Z,X,C released - zoom in/out with optional CTRl hold
	if (!rotate) {
		//const int mod = glutGetModifiers();
		const bool ctrl_pressed = 
				(m_keystate[(unsigned int)'`'] != 0) ||
				(m_keystate[(unsigned int)'~'] != 0);//((mod | GLUT_ACTIVE_CTRL) == 0);

		//fprintf(stderr, "ctrl_pressed: %s\n", sbool(ctrl_pressed));
		m_camera.zoom(wheel_up, ctrl_pressed);
	}
}

void FractalWindow::_st_mouse(int button, int state, int x, int y) {
	if (state != GFX_MOUSE_RELEASE) return;

	//fprintf(stderr, "%s: button: 0x%04X, state: 0x%04X, (x,y) = (%d,%d)\n",
	//  __FUNCTION__, button, state, x, y);

	switch (button) {
		case GFX_MOUSE_LEFT:
			self->_toggleCameraRotate();
			break;
		case GFX_MOUSE_WHEELUP:
			self->_mouseWheel(true);
			break;
		case GFX_MOUSE_WHEELDOWN:
			self->_mouseWheel(false);
			break;
		default: return;
	}

	gfx::setRefresh();
}

void FractalWindow::_st_mouseMove(int x, int y) {
	if (!self->m_paused) {
		self->m_camera.mouseMove(x, y);
		gfx::setRefresh();
	}
}

void FractalWindow::_save() {
	OptAppDrawing opt_app_drawing;

	opt_app_drawing.opt_camera = m_camera.optConst();
	opt_app_drawing.opt_camera_pos = m_camera.posConst();
	opt_app_drawing.opt_display = m_opt_display;
	opt_app_drawing.opt_drawing = m_opt_drawing;
	opt_app_drawing.opt_drawing_creator = m_drawing.drawingCreator();

	if (FractalWindowLoader::get()->save(opt_app_drawing) < 0) {
		fprintf(stderr, "FractalWindow: Failed to save state\n");
	}
	
	opt_app_drawing.opt_drawing_creator = NULL; // prevent from deleting m_drawing.opt_drawing_creator
}

void FractalWindow::_st_keyboard(unsigned char key, int x, int y) {
	//fprintf(stderr, "%s: key = %d (0x%02X)\n", __FUNCTION__, key, key);

	self->m_keystate[key] = 0xFF;

	switch (key) {
	case ' ':
		_st_exportPng();
		return;
		
	case '[':
		if (self->m_curr_light > 0) self->m_curr_light--;
		self->_showSelectedLight();
		return;
	case ']':
		if (self->m_curr_light < sizeof(self->m_light)/sizeof(self->m_light[0])-1) {
			self->m_curr_light++;
			self->_showSelectedLight();
		}
		return;
	case 'u':
		self->_showLightPosInfo();
		return;
	case 'i':
		self->_showLightDiffuseInfo();
		return;
	case 'o':
		self->_showLightSpotDir();
		return;
	case 'j':
		self->_showLightSpotCutOff();
		return;
	case 'k':
		self->_showLightSpotExponent();
		return;
	case 'w':
	case 'W':
		self->m_camera.transUp();
		break;
	case 's':
	case 'S':
		self->m_camera.transDown();
		break;
	case 'a':
	case 'A':
		self->m_camera.transLeft();
		break;
	case 'd':
	case 'D':
		self->m_camera.transRight();
		break;

	case '\t':
		self->m_opt_drawing.toggleDrawMode();
		break;
	case '1':
		if (!self->m_drawing.isCreated()) return;
		self->m_drawing.toggleDisplist();
		break;
	case '2':
		_st_setGlOpt(GL_BLEND, !self->m_is_alpha_enabled, self->m_is_alpha_enabled);
		break;
	case '3':
		_st_setGlOpt(GL_LIGHTING, !self->m_is_lighting_enabled, self->m_is_lighting_enabled);
		break;
	case '4':
		_st_setGlOpt(GL_LIGHT0, !self->m_is_light0_enabled, self->m_is_light0_enabled);
		break;
	case '5':
		_st_setGlOpt(GL_TEXTURE_2D, !self->m_is_texture2d_enabled, self->m_is_texture2d_enabled);
		break;
	case '6':
		_st_setGlOpt(GL_COLOR_MATERIAL, !self->m_is_colormaterial_enabled, self->m_is_colormaterial_enabled);
		break;
	case 19: // Ctrl^S (at least on Windows)
		self->_save();
		break;

	case 27:
		//glutDestroyWindow(self->m_window);
		exit(0);
		return;
	}

	gfx::setRefresh();
}

void FractalWindow::_st_keyboardUp(unsigned char key, int x, int y) {
	self->m_keystate[key] = 0x00;
}

void FractalWindow::_st_keyboardSpecial(int key, int x, int y) {
	//fprintf(stderr, "%s: key = %d (0x%08X)\n", __FUNCTION__, key, key);
	switch (key) {
		case GLUT_KEY_UP:
			self->_mouseWheel(true);
			break;
		case GLUT_KEY_DOWN:
			self->_mouseWheel(false);
			break;
		case GLUT_KEY_F10:
			self->_toggleLight(0);
			return;
		case GLUT_KEY_F11:
			self->_toggleLight(1);
			return;
		case GLUT_KEY_F12:
			self->_toggleLight(2);
			return;
	}
	gfx::setRefresh();
}

/*void FractalWindow::_st_animate() {

}*/

void FractalWindow::_initLights(OptAppDrawingParser& settings) {
	m_curr_light = 0;
	
	vec4::set(m_light[0].diffuse, 	0.8f, 0.8f, 0.8f, 1.0f);
	vec4::set(m_light[1].diffuse, 	0.8f, 0.0f, 0.0f, 1.0f);
	vec4::set(m_light[2].diffuse, 	0.8f, 0.0f, 0.0f, 1.0f);

	vec4::set(m_light[0].pos, 		0.0f, -8.5f, 14.5f, -8.5f);
	vec4::set(m_light[1].pos, 		0.0f, 0.0f, -40.0f, 1.0f);
	vec4::set(m_light[2].pos, 		-2.0f, -2.0f, 40.0f, 1.0f);

	vec3::set(m_light[0].spot_dir, 	0.0f, 0.0f, -1.0f);
	vec3::set(m_light[1].spot_dir, 	0.0f, 0.0f, 1.0f);
	vec3::set(m_light[2].spot_dir, 	0.0f, 0.0f, 1.0f);

	m_light[0].spot_cut_off  = 20.0f;
	m_light[0].spot_exponent = 120.0f;

	m_light[1].spot_cut_off  = 20.0f;
	m_light[1].spot_exponent = 120.0f;

	m_light[2].spot_cut_off  = 20.0f;
	m_light[2].spot_exponent = 120.0f;
	
	const float matSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	//const float lightPosition[] = {1.0f, 1.0f, 1.0f, 0.0f};
	//const float whiteLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
	const float lModelAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};
	
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);			// sets specular material color
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0f);					// sets shininess
	
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);			// sets light position
	
	// set specular and diffuse color of light
	glLightfv(GL_LIGHT0, GL_SPECULAR, settings.opt_app_drawing.opt_drawing_creator->color.dataConst());
	glLightfv(GL_LIGHT0, GL_DIFFUSE, settings.opt_app_drawing.opt_drawing_creator->color.dataConst());
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lModelAmbient);		// global ambient light 
	
	_st_setGlOpt(GL_LIGHT0, true, m_light[0].enabled);
	_st_setGlOpt(GL_LIGHTING, false, m_is_lighting_enabled);
	
	_st_setGlOpt(GL_COLOR_MATERIAL, true, m_is_colormaterial_enabled);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);			// tell opengl glColor3f effects the ambient and diffuse properties of material
	//----------- END: Variables & method calls added for Lighting Test -----------//
	
	_st_setGlOpt(GL_LIGHT1, false, m_light[1].enabled);
	_st_setGlOpt(GL_LIGHT2, false, m_light[2].enabled);
	_st_setGlOpt(GL_TEXTURE_2D, false, m_is_texture2d_enabled);
}

inline void FractalWindow::_applyLights() const {
	for (unsigned int i = 0; i < 3; i++) {
		if (m_light[i].enabled) {
			glLightfv(LIGHT_IDS[i], GL_DIFFUSE, m_light[i].diffuse);
			glLightfv(LIGHT_IDS[i], GL_POSITION, m_light[i].pos);
			
			//glLightf(LIGHT_IDS[i], GL_SPOT_CUTOFF,  m_light[i].spot_cut_off);
			//glLightf(LIGHT_IDS[i], GL_SPOT_EXPONENT, m_light[i].spot_exponent);
		}
	}
}

void FractalWindow::_st_display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (self->m_drawing.isCreated()) {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		self->m_camera.applyLook();
		
		self->_applyLights();
		
		//glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

		/*Log_debug("Rotation: (%4.2f, %4.2f, %4.2f)",
		self->m_opt_drawing.rotation.xConst(),
		self->m_opt_drawing.rotation.yConst(),
		self->m_opt_drawing.rotation.zConst());*/
		glRotatef(self->m_opt_drawing.rotation.xConst(), 1.0f, 0.0f, 0.0f);
		glRotatef(self->m_opt_drawing.rotation.yConst(), 0.0f, 1.0f, 0.0f);
		glRotatef(self->m_opt_drawing.rotation.zConst(), 0.0f, 0.0f, 1.0f);

		self->m_drawing.draw();
	}

	gfx::swapBuffers();
}

void FractalWindow::_st_resize(int new_w, int new_h) {
	double ratio = new_w;

	if (new_h)
		ratio /= new_h;
	else
		new_h = 1;
		
	self->m_opt_display.disp_geom.w = new_w;
	self->m_opt_display.disp_geom.h = new_h;

	//fprintf(stderr, "Resized to %d x %d [ratio: %f]\n", new_w, new_h, ratio);

	glViewport(0, 0, new_w, new_h); // minx,miny,maxx,maxy
	glMatrixMode(GL_PROJECTION); // kolejne operacje dotyczą macierzy GL_PROJECTION

	// Set the camera perspective
	glLoadIdentity();                   // reset the camera // wyczyszczenie macierzy GL_PROJECTION

	gluPerspective(45.0,               // kąt rozwarcia "obiektywu" kamery w kierunku pionowym
		(double)new_w / (double)new_h, // stosunek szerokości i wysokości okna (pozwala na prawidłowe przybliżanie i oddalanie)
		0.05,                // the near z clipping coord. (obiekty bliższe niż tyle jednostek nie są rysowane)
		200.0);             // the far  z clipping coord. (obiekty dalse niż tyle jednostek nie są rysowane)

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void FractalWindow::_st_sigint(int sig) {
	exit(0);
}

int FractalWindow::init() {
	if (self != NULL) {
		fprintf(stderr, "FractalWindow::init() Already initialized\n");
		return -1;
	}

	self = this;
	m_paused = true;
	memset(m_keystate, 0, sizeof(m_keystate)); // no keys are pressed
	return 0;
}

void FractalWindow::start() const {
	(void)signal(SIGINT, FractalWindow::_st_sigint);
	glutMainLoop();
}

// m_opt_display must be set at this point
void FractalWindow::initGfx(OptAppDrawingParser& settings, ProgramArgs& args) {
	_updateOptAppDrawingParser(settings);

	_initGlutWindow(settings, args);

	_initGraphics(settings);

	_st_setCallbacks();
}

void FractalWindow::_initGlutWindow(OptAppDrawingParser& settings, ProgramArgs& args) {
	glutInit(&args.argc, args.argv_non_utf16);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(m_opt_display.disp_geom.w, m_opt_display.disp_geom.h);
	glutInitWindowPosition(m_opt_display.disp_geom.x, m_opt_display.disp_geom.y);

#ifdef UNICODE
	std::wstring name;
	FractalWindowLoader::get()->getName(name);
	char* c_name = unicodeToMultibyte(name.c_str(), name.length());
#else
	std::string name;
	FractalWindowLoader::get()->getName(name);
	const char* c_name = name.c_str();
#endif
	m_window = glutCreateWindow(c_name); // create window

#ifdef UNICODE
	delete[] c_name;
#endif
}

void FractalWindow::_updateOptAppDrawingParser(const OptAppDrawingParser& settings) {
	m_camera.opt() = settings.opt_app_drawing.opt_camera;
	m_camera.pos() = settings.opt_app_drawing.opt_camera_pos;
	m_opt_display = settings.opt_app_drawing.opt_display;
	m_opt_drawing = settings.opt_app_drawing.opt_drawing;
}

void FractalWindow::_st_setCallbacks() {
	glutMouseFunc(_st_mouse);
	glutPassiveMotionFunc(_st_mouseMove);
	glutKeyboardFunc(_st_keyboard);
	glutKeyboardUpFunc(_st_keyboardUp);
	glutSpecialFunc(_st_keyboardSpecial);
	glutDisplayFunc(_st_display);
	glutReshapeFunc(_st_resize);
	//glutIdleFunc(_st_animate);
}

void FractalWindow::_initGraphics(OptAppDrawingParser& settings) {
	// initialize some constant OpenGL settings
	glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading
	glClearDepth(1.0f);                                 // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);                             // The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really Nice Perspective Calculations

	// alpha channel
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*
	const GLfloat global_ambient[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	const GLfloat diffuseLight[]   = { 0.7f, 0.7f, 0.7f, 1.0f };


	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 25.0f);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 75.0f);
	//glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	_st_setGlOpt(GL_BLEND, true, m_is_alpha_enabled);
	_st_setGlOpt(GL_LIGHTING, true, m_is_lighting_enabled);
	_st_setGlOpt(GL_LIGHT0, true, m_is_light0_enabled);
	_st_setGlOpt(GL_TEXTURE_2D, true, m_is_texture2d_enabled);
	_st_setGlOpt(GL_COLOR_MATERIAL, true, m_is_colormaterial_enabled);
	*/

	_initLights(settings);
	
	_st_setGlOpt(GL_BLEND, true, m_is_alpha_enabled);

	// initialize variable settings
	m_opt_display.apply();
}
