/*
 * File:   OptDrawingCreatorFirst.hpp
 * Author: darek
 *
 * Created on 11 December 2014, 17:20
 */
#ifndef QFRACTAL_OPTDRAWINGCREATORFIRST_HPP
#define QFRACTAL_OPTDRAWINGCREATORFIRST_HPP

#include "ISerializable.hpp"
#include "utils.hpp"
#include "Color.hpp"
#include "OptDrawingCreator.hpp"

class OptDrawingCreatorFirst : public OptDrawingCreator {
protected:
	virtual int	customReadText(Parser& parser);
	virtual int	customSaveText(Parser& parser) const;
	virtual int customReadBin(FILE* f);
	virtual int customSaveBin(FILE* f) const;
	virtual void customSetDefault();
	virtual void customSet(const OptDrawingCreator& other);
	
public:
	float		elem_size_percent;
	float		alpha_2;
	bool		color_negative_2;
	
	virtual ~OptDrawingCreatorFirst() { }
};

#endif
