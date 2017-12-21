/*
 * File:   OptDrawingCreator.hpp
 * Author: darek
 *
 * Created on 23 November 2014, 23:29
 */
#ifndef QFRACTAL_DRAWING_OPTDRAWINGCREATOR_HPP
#define QFRACTAL_DRAWING_OPTDRAWINGCREATOR_HPP

#include "ISerializable.hpp"
#include "utils.hpp"
#include "Color.hpp"

class OptDrawingCreator : public ISerializable {
protected:
	virtual int	customReadText(Parser& parser) = 0;
	virtual int	customSaveText(Parser& parser) const = 0;
	virtual int customReadBin(FILE* f) = 0;
	virtual int customSaveBin(FILE* f) const = 0;
	virtual void customSetDefault() = 0;
	virtual void customSet(const OptDrawingCreator& other) = 0;

public: /// common fields
	// algorithm version
	unsigned int	version;

	// point color
	Color4f			color;

	// inverse color ?
	bool			color_negative;

	// initial display list index
	unsigned int	disp_list_ind;

	virtual ~OptDrawingCreator() { }

	// read common + custom fields
	virtual int readText(Parser& parser);

	// read common + custom fields
	virtual void readBin(FILE* f);

	virtual int saveText(Parser& parser) const;

	virtual void saveBin(FILE* f) const;

	void setDefault();

	void set(const OptDrawingCreator& other);

	/// create new OptDrawing of appropiate type (must be deleted by caller)
	/// returns NULL on error
	static OptDrawingCreator* readNewByText(Parser& parser);

	/// create new OptDrawing of appropiate type (must be deleted by caller)
	/// returns NULL on error
	static OptDrawingCreator* readNewByBin(FILE* f);

	/// create new OptDrawing of appropiate type (must be deleted by caller)
	/// returns NULL on error
	static OptDrawingCreator* newByVersion(unsigned int version);

	/// create new OptDrawing of appropiate type (must be deleted by caller)
	/// returns NULL on error
	static OptDrawingCreator* newByCopy(const OptDrawingCreator& other);

private:
	/// read common fields
	/// @note version is already read
	/// @see readNewByText
	int _readTextCommon(Parser& parser);

	/// read common fields
	/// @note version is already read
	/// @see readNewByBin()
	int _readBinCommon(FILE* f);

	/// save common fields (also version)
	int _saveTextCommon(Parser& parser) const;

	/// save common fields (also version)
	int _saveBinCommon(FILE* f) const;

	static void _st_unexpectedItem(const char* id, const char* id_type);

	static OptDrawingCreator* _st_readText(Parser& parser, unsigned int version);

};

#endif
