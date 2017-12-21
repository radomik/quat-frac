/*
* File:   DrawingCreatorFirst.hpp
* Author: darek
*
* Created on 10 December 2014, 18:00
*/
#ifndef QFRACTAL_DRAWING_DRAWINGCREATORFIRST_HPP
#define QFRACTAL_DRAWING_DRAWINGCREATORFIRST_HPP

#include "utils.hpp"
#include "DrawingCreator.hpp"
#include "OptGenerator.hpp"
#include "gfx.hpp"

class GLdrawing;
class OptDrawingCreator;
class OptDrawingCreatorFirst;
class Vec3f;

class DrawingCreatorFirst : public DrawingCreator 
{
	GLuint*					 m_indices;
	GLfloat*				 m_vertices;
	GLubyte*				 m_colors;
	ValueVec3f				 m_step;
	
public:
	DrawingCreatorFirst(const OptGenerator& opt) 
		: DrawingCreator(opt), 
		  m_indices(NULL), m_vertices(NULL), m_colors(NULL) { }
		
	virtual			~DrawingCreatorFirst();

protected:
	virtual unsigned int getVersion() const  { return 1; }
	virtual const char*  getName() const { return "DrawingCreatorFirst (vertex arrays)"; }
	virtual size_t		 getDrawingElementByteSize() const { return VEC3F_BIN_SIZE + sizeof(range_iter_t); }
	virtual int 		 checkOpt(const OptDrawingCreator& opt) const;
	//virtual void		 destroyDisplayObjects(GLdrawing& drawing);

	virtual int createCustom( 
		GLdrawing&					drawing,
		unsigned char*				file_data,
		const OptDrawingCreator&	opt_drawing,
		unsigned int				count_data_points);

private:
	static const GLubyte INDICES_COUNT_PER_ELEMENT;
	static const GLubyte VERTEX_COUNT_PER_ELEMENT;
	static const GLubyte COLOR_CHANNEL_COUNT_PER_VERTEX;
	static const GLubyte VERTEX_COORDS_COUNT_PER_ELEMENT;
	static const GLubyte COLOR_CHANNEL_COUNT_PER_ELEMENT;

	int _createIndices(unsigned int count_data_points);

	int _allocateVerticesAndColors(unsigned int	count_data_points);

	void _createVerticesAndColors(
		unsigned char*					file_data,
		const OptDrawingCreatorFirst&	opt_drawing,
		unsigned int					count_data_points);
};

#endif
