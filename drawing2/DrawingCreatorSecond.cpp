#include "stdafx.h"
#include "DrawingCreatorSecond.hpp"
#include "BinSerialization.hpp"
#include "GLdrawing.hpp"
#include "Vec.hpp"
#include "Quat.hpp"
#include "OptDrawingCreatorSecond.hpp"
#include "GeomMath.hpp"
#include "timing.h"

const GLubyte DrawingCreatorSecond::VERTEX_COUNT_PER_ELEMENT = 1;
const GLubyte DrawingCreatorSecond::VERTEX_COORDS_COUNT_PER_ELEMENT = VERTEX_COUNT_PER_ELEMENT * 3;
const GLubyte DrawingCreatorSecond::COLOR_CHANNEL_COUNT_PER_ELEMENT = VERTEX_COUNT_PER_ELEMENT * 4;

DrawingCreatorSecond::~DrawingCreatorSecond() {
	if (m_vertices) {
		//fprintf(stderr, "[DBG] %s @ %d [DEL] %p (GLfloat[] m_vertices)\n", __FILE__, __LINE__, m_vertices);
		delete[] m_vertices; m_vertices = NULL;
	}
	if (m_colors) {
		//fprintf(stderr, "[DBG] %s @ %d [DEL] %p (GLubyte[] m_colors)\n", __FILE__, __LINE__, m_colors);
		delete[] m_colors; m_colors = NULL;
	}
}

int DrawingCreatorSecond::checkOpt(const OptDrawingCreator& opt) const {
	const OptDrawingCreatorSecond *opt_draw =
		dynamic_cast< const OptDrawingCreatorSecond* >(&opt);
	if (!opt_draw) {
		fprintf(stderr, "DrawingCreatorSecond: Cannot cast '%s' to \
OptDrawingCreatorSecond\n", typnam(opt));
		return -1;
	}
	return 0;
}

int DrawingCreatorSecond::_allocateVerticesAndColors(
	unsigned int					count_data_points)
{
	//reset_and_start_timer();

	// count vertices (multiplied by count of coords)
	size_t count_vertices = count_data_points * VERTEX_COORDS_COUNT_PER_ELEMENT;

	char strSize[64];
	formatBytes(count_vertices*sizeof(float), strSize, sizeof(strSize));
	fprintf(stderr, "Allocating: %s data for vertices\n", strSize);

	// Allocate array for vertices
	try {
		m_vertices = new GLfloat[count_vertices];
		//fprintf(stderr, "[DBG] %s @ %d [NEW] %p (GLfloat[] m_indices)\n", __FILE__, __LINE__, m_vertices);
	}
	catch (const std::bad_alloc& e) {
		SysError ex;
		fprintf(stderr, "Failed to allocate %s data for vertices [what: %s, errno: %s]\n", strSize, e.what(), ex.msg);
		return -1;
	}

	// Count for colors (multiplied by count of channels)
	size_t count_colors = count_data_points * COLOR_CHANNEL_COUNT_PER_ELEMENT;

	// Allocate array for colors
	formatBytes(count_colors*sizeof(GLubyte), strSize, sizeof(strSize));
	fprintf(stderr, "Allocating: %s data for colors\n", strSize);
	try {
		m_colors = new GLubyte[count_colors];
		//fprintf(stderr, "[DBG] %s @ %d [NEW] %p (GLubyte[] m_colors)\n", __FILE__, __LINE__, m_colors);
	}
	catch (const std::bad_alloc& e) {
		SysError ex;
		fprintf(stderr, "Failed to allocate %s data for colors [what: %s, errno: %s]\n", strSize, e.what(), ex.msg);
		return -1;
	}

	//fprintf(stderr, "Allocated vertex and color arrays in %.3f mcycles\n", get_elapsed_mcycles());
	return 0;
}

void DrawingCreatorSecond::_createVerticesAndColors(
		unsigned char*					file_data,
		unsigned int					count_data_points,
		const OptDrawingCreatorSecond&	opt_drawing)
{
	ValueQuatf		q;

	const Color4f	base_color = opt_drawing.color;
	GLfloat			*vertices = m_vertices;
	GLubyte			*colors = m_colors;
	unsigned int    count;
	float			col_scale;
	GLubyte			col_ch, color[4];
	range_iter_t	value;

	//fprintf(stderr, "DrawingCreatorSecond: Creating drawing\n");

	// alpha is common for all points, convert from [0;1] scale to 0-255 scale
	color[3] = (GLubyte)(base_color.aConst() * 255.0f);

	unsigned char	*data_ptr = file_data;
	for (count = 1; count <= count_data_points; count++) {
		// Read data item and its value from memory
		memcpy(q.v, data_ptr, sizeof(q.v)); data_ptr += sizeof(q.v);
		memcpy(&value, data_ptr, sizeof(value)); data_ptr += sizeof(value);

		// Convert 4-element vector from data item to 3-element point to be displayed
		// and write it to vertices array
		for (unsigned int k = 0; k < 3; k++) {
			*(vertices++) = q.v[ opt_drawing.coord_map[k] ];
		}

		// color is a count of recursive function iteration in point
		// multiplied by some value and truncated to unsigned char
		col_ch = (unsigned char)(value * opt_drawing.color_multiplier);

		// make this color negative if requested
		if (opt_drawing.color_negative)
			col_ch = 255 - col_ch;

		// col_ch = [0...255] convert it to [0.0f ... 1.0f]
		col_scale = (float)col_ch / 255.0f;

		// calculate point color using base color and scale (multiply r, g, b, leave: a)
		for (unsigned int i = 0; i < 3; i++) {
			color[i] = (GLubyte)(base_color[i] * col_scale * 255.0f);
		}

		// write color (with alpha) to colors array
		memcpy(colors, color, sizeof(color));
		colors += sizeof(color);

		if (count % 10000 == 0)
			std::cerr << "\rRead " << count << " data items";
	}

	std::cerr << "\rRead " << count << " data items" << std::endl;
	//fprintf(stderr, "Created vertex and colors data in %.3f mcycles\n", get_elapsed_mcycles());
}

int DrawingCreatorSecond::createCustom(
	GLdrawing&					drawing,
	unsigned char*				file_data,
	const OptDrawingCreator&	opt_drawing,
	unsigned int				count_data_points)
{
	const char *self_name = "DrawingCreatorSecond::createCustom:";

	const OptDrawingCreatorSecond *opt_draw =
		dynamic_cast< const OptDrawingCreatorSecond* >(&opt_drawing);

	// Create vertices - for first and second drawing item
	// and colors - for second drawing item

	// calculate step
	vec4::set(m_step, m_opt.range.x.step, m_opt.range.y.step, m_opt.range.z.step, m_opt.range.w.step);
	if (m_opt.mul_range_by_pi) vec4::mul(m_step, (float)M_PI);

	// allocate arrays for vertices and colors
	if (_allocateVerticesAndColors(count_data_points) < 0)
	{
		fprintf(stderr, "%s Failed to allocate vertices and colors\n", self_name);
		return -1;
	}

	// fill arrays of vertices and colors
	_createVerticesAndColors(file_data, count_data_points, *opt_draw);

	// initialize drawing
	if (drawing.init(1) < 0) {
		fprintf(stderr, "%s Failed to initialize drawing\n", self_name);
		return -1;
	}

	// Add first drawing item
	GLdrawingItem *item = drawing.getItem(0);

	item->vertices = m_vertices;
	item->indices = NULL;
	item->colors = m_colors;
	item->count = count_data_points;
	item->mode = GL_POINTS;
	item->color_channels = 4;

	return 0;
}
/*
void DrawingCreatorSecond::destroyDisplayObjects(GLdrawing& drawing) {
	GLdrawingItem *item = drawing.getItem(0);

	// destroy first display object if exists
	if (item) {
		// delete only vertices and colors, there are no indices for points
		//fprintf(stderr, "[DBG] %s @ %d [DEL] %p (GLfloat[] item->vertices)\n", __FILE__, __LINE__, item->vertices);
		delete[] item->vertices; item->vertices = NULL;

		//fprintf(stderr, "[DBG] %s @ %d [DEL] %p (GLubyte[] item->colors)\n", __FILE__, __LINE__, item->colors);
		delete[] item->colors; item->colors = NULL;
	}
}
*/
