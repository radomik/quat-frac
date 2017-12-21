#include "stdafx.h"
#include "DrawingCreatorFirst.hpp"
#include "BinSerialization.hpp"
#include "GLdrawing.hpp"
#include "Vec.hpp"
#include "OptDrawingCreatorFirst.hpp"
#include "GeomMath.hpp"
#include "timing.h"

const GLubyte DrawingCreatorFirst::INDICES_COUNT_PER_ELEMENT = 18;
const GLubyte DrawingCreatorFirst::VERTEX_COUNT_PER_ELEMENT = 7;
const GLubyte DrawingCreatorFirst::COLOR_CHANNEL_COUNT_PER_VERTEX  = 4;
const GLubyte DrawingCreatorFirst::VERTEX_COORDS_COUNT_PER_ELEMENT = VERTEX_COUNT_PER_ELEMENT * 3;
const GLubyte DrawingCreatorFirst::COLOR_CHANNEL_COUNT_PER_ELEMENT = VERTEX_COUNT_PER_ELEMENT * COLOR_CHANNEL_COUNT_PER_VERTEX;

DrawingCreatorFirst::~DrawingCreatorFirst() {
	if (m_indices) {
		//fprintf(stderr, "[DBG] %s @ %d [DEL] %p (GLuint[] m_indices)\n", __FILE__, __LINE__, m_indices);
		delete[] m_indices; m_indices = NULL;
	}
	if (m_vertices) {
		//fprintf(stderr, "[DBG] %s @ %d [DEL] %p (GLfloat[] m_vertices)\n", __FILE__, __LINE__, m_vertices);
		delete[] m_vertices; m_vertices = NULL;
	}
	if (m_colors) {
		//fprintf(stderr, "[DBG] %s @ %d [DEL] %p (GLubyte[] m_colors)\n", __FILE__, __LINE__, m_colors);
		delete[] m_colors; m_colors = NULL;
	}
}

int DrawingCreatorFirst::checkOpt(const OptDrawingCreator& opt) const {
	const OptDrawingCreatorFirst *opt_draw = 
		dynamic_cast< const OptDrawingCreatorFirst* >(&opt);
	if (! opt_draw) {
		fprintf(stderr, "DrawingCreatorFirst: Cannot cast '%s' to \
OptDrawingCreatorFirst\n", typeid(opt).name());
		return -1;
	}
	return 0;
}

int DrawingCreatorFirst::_createIndices(unsigned int count_data_points) {
	//reset_and_start_timer();

	// Indices for first drawing element, subsequent elements are created from previous
	// by adding some offset (count of vertex in drawing element) 
	const GLuint FIRST_INDICES[INDICES_COUNT_PER_ELEMENT] = {
		0, 1, 2,
		0, 2, 3,
		0, 6, 4,
		0, 3, 6,
		3, 2, 6,
		6, 2, 5
	};
	
	// Count of elements in indices array
	size_t count_indices = count_data_points * INDICES_COUNT_PER_ELEMENT;

	char strSize[64];
	formatBytes(count_indices*sizeof(GLuint), strSize, sizeof(strSize));
	
	// Allocate indices array
	fprintf(stderr, "Allocating: %s data for indices\n", strSize);
	try {
		m_indices = new GLuint[count_indices];
		//fprintf(stderr, "[DBG] %s @ %d [NEW] %p (GLuint[] m_indices)\n", __FILE__, __LINE__, m_indices);
	}
	catch (const std::bad_alloc& e) {
		SysError ex;
		fprintf(stderr, "Failed to allocate %s data for indices [what: %s, errno: %s]\n", strSize, e.what(), ex.msg);
		return -1;
	}

	// Fill first indices for first drawing element
	memcpy(m_indices, FIRST_INDICES, sizeof(FIRST_INDICES));
	
	// Fill all remaining indices
	GLuint *ind_src = m_indices;
	GLuint *ind_dest = m_indices + INDICES_COUNT_PER_ELEMENT;
	
	for (unsigned int elem = 1; elem < count_data_points; elem++) {
		for (GLubyte k = 0; k < INDICES_COUNT_PER_ELEMENT; k++) {
			*(ind_dest++) = *(ind_src++) + VERTEX_COUNT_PER_ELEMENT;
		}
	}

	//fprintf(stderr, "Created indices in %.3f mcycles\n", get_elapsed_mcycles());

	return 0;
}

int DrawingCreatorFirst::_allocateVerticesAndColors(unsigned int count_data_points)
{
	//reset_and_start_timer();

	// count vertices (multiplied by count of coords) 
	// and the same count for colors (multiplied by count of channels)
	size_t count_vertices = count_data_points * VERTEX_COORDS_COUNT_PER_ELEMENT;
	size_t count_colors = count_data_points * COLOR_CHANNEL_COUNT_PER_ELEMENT;
	
	char strSize[64];
	formatBytes(count_vertices*sizeof(float), strSize, sizeof(strSize));
	fprintf(stderr, "Allocating: %s data for vertices\n", strSize);

	// Allocate array for vertices
	try {
		m_vertices = new GLfloat[count_vertices];
		//fprintf(stderr, "[DBG] %s @ %d [NEW] %p (GLfloat[] m_vertices)\n", __FILE__, __LINE__, m_vertices);
	}
	catch (const std::bad_alloc& e) {
		SysError ex;
		fprintf(stderr, "Failed to allocate %s data for vertices [what: %s, errno: %s]\n", strSize, e.what(), ex.msg);
		return -1;
	}

	// Allocate array for colors
	formatBytes(count_colors*sizeof(GLubyte), strSize, sizeof(strSize));
	fprintf(stderr, "Allocating: %s data for colors\n", strSize);
	try {
		m_colors = new GLubyte[count_colors];
		//fprintf(stderr, "[DBG] %s @ %d [NEW] %p (GLfloat[] m_colors)\n", __FILE__, __LINE__, m_colors);
	}
	catch (const std::bad_alloc& e) {
		SysError ex;
		fprintf(stderr, "Failed to allocate %s data for colors [what: %s, errno: %s]\n", strSize, e.what(), ex.msg);
		return -1;
	}

	//fprintf(stderr, "Allocated vertex and color arrays in %.3f mcycles\n", get_elapsed_mcycles());
	return 0;
}

void DrawingCreatorFirst::_createVerticesAndColors(
		unsigned char*					file_data,
		const OptDrawingCreatorFirst&	opt_drawing,
		unsigned int					count_data_points)
{
	const uint32_t	MAX_COLOR = 0x00FFFFFF;
	const uint8_t	XRGB_TO_RGBX_SHIFT = 8;
	
	//reset_and_start_timer();

	// Color map (for now - constant, in range of green color)
	// NOTE: in order to use this color map, change:
	// COLOR_CHANNEL_COUNT_PER_VERTEX = 3 (this map uses only 3 color channels - no alpha channel)
	//  on the end of createCustom()
	/*#define COLOR_MAP_COUNT 9
	static const GLubyte COLOR_MAP[COLOR_MAP_COUNT][3] = {
		{ 0, 100, 0 },
		{ 0, 120, 0 },
		{ 0, 140, 0 },
		{ 0, 160, 0 },
		{ 0, 180, 0 },
		{ 0, 200, 0 },
		{ 0, 220, 0 },
		{ 0, 240, 0 },
		{ 0, 255, 0 },
	};*/
	uint32_t		color_rgba;
	uint8_t			alpha = (uint8_t)(opt_drawing.alpha_2 * 255.0f);
	// Create some constant from current iteration settings in order to use color map
	//const range_iter_t    min_value = m_opt.iter.min;
	//const float           dx = (float)(m_opt.iter.max - min_value + 1) / COLOR_MAP_COUNT;

	ValueVec3f		a, b;
	unsigned int	count;//, color_ind;
	unsigned char	*data_ptr = file_data;
	GLubyte			*colors = m_colors;
	GLfloat			*vertices = m_vertices;
	range_iter_t	value;
	range_iter_t	iter_range = m_opt.iter.max - m_opt.iter.min;

	// Create vertices and colors
	//fprintf(stderr, "Using generic version to make vertices and colors\n");
	//fprintf(stderr, "alpha: %f = %d\n", opt_drawing.alpha_2, alpha);
	//fprintf(stderr, "color_negative_2: %s\n", syesno(opt_drawing.color_negative_2));

	for (count = 1; count <= count_data_points; count++) 
	{
		// Read data item and its value from memory
		memcpy(a.v, data_ptr, sizeof(a.v)); data_ptr += sizeof(a.v);
		memcpy(&value, data_ptr, sizeof(value)); data_ptr += sizeof(value);

		// Calculate opposite point to data item
		vec3::add(b, a, m_step);
		
		// Calculate color from full RGB range -> color_rgba = 0x00RRGGBB
		color_rgba = (uint32_t)(((double)(value - m_opt.iter.min)/ iter_range) * MAX_COLOR);
		//color_rgba = 0x00FF0000;
		
		// Invert color if needed -> color_rgba = 0x00FFFFFF - color_rgba
		if (opt_drawing.color_negative_2) {
			color_rgba = MAX_COLOR - color_rgba;
		}
		
		// Convert 0x00RRGGBB -> 0xRRGGBB00
		color_rgba <<= XRGB_TO_RGBX_SHIFT;
		
		// fix value of alpha (common for all points - from parameters) -> color_rgba = 0xRRGGBBAA
		color_rgba  |= alpha;
		
		// fill that color for all of seven points of drawing element
		for (unsigned int k = 0; k < VERTEX_COUNT_PER_ELEMENT; k++) {
			memcpy(colors, &color_rgba, sizeof(color_rgba));
			colors += sizeof(color_rgba);
		}

		/*// Calculate index of color from pallete using value of data item
		color_ind = ((unsigned int)((float)(value - min_value) / dx)) % COLOR_MAP_COUNT;

		// fill that color for all of seven points of drawing element
		for (unsigned int k = 0; k < VERTEX_COUNT_PER_ELEMENT; k++) {
			memcpy(colors, &COLOR_MAP[color_ind], sizeof(COLOR_MAP[color_ind]));
			colors += sizeof(COLOR_MAP[color_ind]);
		}*/

		// Create and save all of seven points of drawing element
		*(vertices++) = a.x;	// v0
		*(vertices++) = a.y;
		*(vertices++) = b.z;

		*(vertices++) = b.x;	// v1
		*(vertices++) = a.y;
		*(vertices++) = b.z;

		*(vertices++) = b.x;	// v2
		*(vertices++) = a.y;
		*(vertices++) = a.z;

		*(vertices++) = a.x;	// v3
		*(vertices++) = a.y;
		*(vertices++) = a.z;

		*(vertices++) = a.x;	// v4
		*(vertices++) = b.y;
		*(vertices++) = b.z;

		*(vertices++) = b.x;	// v5
		*(vertices++) = b.y;
		*(vertices++) = a.z;

		*(vertices++) = a.x;	// v6
		*(vertices++) = b.y;
		*(vertices++) = a.z;
		
		if (count % 10000 == 0)
			std::cerr << "\rRead " << count << " data items";
	}

	std::cerr << "\rRead " << count << " data items" << std::endl;
	//fprintf(stderr, "Created vertex and colors data in %.3f mcycles\n", get_elapsed_mcycles());
}

int DrawingCreatorFirst::createCustom( 
		GLdrawing&					drawing, 
		unsigned char*				file_data,
		const OptDrawingCreator&	opt_drawing,
		unsigned int				count_data_points) 
{
	const char		*self_name = "DrawingCreatorFirst::createCustom:";
	
	const OptDrawingCreatorFirst *opt_draw = 
		dynamic_cast< const OptDrawingCreatorFirst* >(&opt_drawing);

	// calculate step
	vec3::set(m_step, m_opt.range.x.step, m_opt.range.y.step, m_opt.range.z.step);
	if (m_opt.mul_range_by_pi) vec3::mul(m_step, (float)M_PI);

	if (!Float::equ(opt_draw->elem_size_percent, 100.0f)) {
		float d = opt_draw->elem_size_percent / 100.0f;
		vec3::mul(m_step, d);
	}

	// create indices - may be reused for two VBOs, but should not be deleted TWICE
	if (_createIndices(count_data_points) < 0) {
		fprintf(stderr, "%s Failed to create indices\n", self_name);
		return -1;
	}

	// Create vertices - for first and second drawing item
	// and colors - for second drawing item
	// allocate arrays for vertices and colors
	if (_allocateVerticesAndColors(count_data_points) < 0) 
	{
		fprintf(stderr, "%s Failed to allocate vertices and colors\n", self_name);
		return -1;
	}

	// fill arrays of vertices and colors
	_createVerticesAndColors(file_data, *opt_draw, count_data_points);

	// set data (indices and vertices)
	if (drawing.init(2) < 0) {
		fprintf(stderr, "%s Failed to initialize drawing\n", self_name);
		return -1;
	}

	const unsigned int indices_count = INDICES_COUNT_PER_ELEMENT * count_data_points;

	// Add first drawing item
	GLdrawingItem *item = drawing.getItem(0);

	item->vertices = m_vertices;
	item->indices = m_indices;
	item->colors = NULL;
	item->count = indices_count;
	item->mode = GL_TRIANGLES;

	// Set color (one) for first drawing item
	if (opt_drawing.color_negative)
	{
		vec4::set(item->color.v,
			1.0f - opt_drawing.color.rConst(), // inverse R
			1.0f - opt_drawing.color.gConst(), // inverse G
			1.0f - opt_drawing.color.bConst(), // inverse B
			opt_drawing.color.aConst());	   // no change alpha
	}
	else {
		vec4::set(item->color.v, opt_drawing.color.dataConst());
	}
	
	// Add second drawing item
	item = drawing.getItem(1);
	item->vertices = m_vertices; // the same vertices
	item->indices = m_indices; // the same indices
	item->colors = m_colors; // this drawing item has color array
	item->count = indices_count; // the same count of vertices
	item->mode = GL_TRIANGLES; // the same drawing mode
	item->color_channels = COLOR_CHANNEL_COUNT_PER_VERTEX; 
	
	return 0;
}
/*
void DrawingCreatorFirst::destroyDisplayObjects(GLdrawing& drawing) {
	GLdrawingItem *item = drawing.getItem(0);

	// destroy first display object if exists
	if (item) {
		//fprintf(stderr, "[DBG] %s @ %d [DEL] %p (GLfloat[] item->vertices)\n", __FILE__, __LINE__, item->vertices);
		delete[] item->vertices; item->vertices = NULL;
		//fprintf(stderr, "[DBG] %s @ %d [DEL] %p (GLuint[] item->indices)\n", __FILE__, __LINE__, item->indices);
		delete[] item->indices; item->indices = NULL;
	}

	// destroy second display object if exists
	item = drawing.getItem(1);
	if (item) {
		//fprintf(stderr, "[DBG] %s @ %d [DEL] %p (GLubyte[] item->colors)\n", __FILE__, __LINE__, item->colors);
		delete[] item->colors; item->colors = NULL;
		item->vertices = NULL; // indices and vertices, shared with [0] item
		item->indices = NULL;  // was already deleted above
	}
}
*/
