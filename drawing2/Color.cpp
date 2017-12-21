#include "stdafx.h"
#include "Color.hpp"
#include "FloatingPoint.hpp"
#include "Name.hpp"
#include "Parser.hpp"
#include "BinSerialization.hpp"
#include "GeomMath.hpp"

bool Color4f::operator ==(const Color4f& c) const {
	return vec4::equals(dataConst(), c.dataConst());
}

int Color4f::readText(Parser& parser) {
	const char *self_name = "Color4f";

	Float val_float;

	bool _rd[Color4IndCount] = { false, false, false, false };

	unsigned int i;
	int ret;

	while (!parser.isEof()) {
		if (parser.parseLine() < 0)
			return -1;

		if (parser.isClosureEnd()) break;

		if (parser.isClosureBegin()) {
			parser.unexpectedClosure(self_name);
			return -1;
		}
		else {
			if (parser.isToken()) {
				for (i = 0; i < Color4IndCount; i++) {
					if ((ret = parser.parseToken(val_float, Name::_rgba[i], &_rd[i], self_name)) < 0) return -1;
					if ((ret > 0) && (_rd[i])) {
						if (!validChannel(val_float.value))
							return _invalChannel(Name::_rgba[i], val_float.value);
						operator[](i) = val_float.value; break;
					}
				}

				if (i < 4) continue;

				parser.unexpectedToken(self_name);
				return -1;
			}
		}
	}

	if (parser.isFull())
	{
		for (i = 0; i < Color4IndCount; i++) {
			if (!_rd[i]) {
				fprintf(stderr, "%s: Partially parsed:\n\
Have{r,g,b,a}={%d,%d,%d,%d}\n", self_name, _rd[0], _rd[1], _rd[2], _rd[3]);
				return -1;
			}
		}
	}

	return 0;
}

int Color4f::saveText(Parser& parser) const {
	Float val_float;

	for (unsigned int i = 0; i < Color4IndCount; i++) {
		val_float.value = operator[](i);
		if (parser.writePrimitive(val_float, Name::_rgba[i]) < 0) return -1;
	}

	return 0;
}

int Color4f::readBin(FILE *f) {
	for (unsigned int i = 0; i < Color4IndCount; i++) {
		if (BinSerialization::readValue<float>(f, operator[](i)) < 0) return -1;
	}

	return 0;
}

int Color4f::saveBin(FILE *f) const {
	for (unsigned int i = 0; i < Color4IndCount; i++) {
		if (BinSerialization::writeValue<float>(f, operator[](i)) < 0) return -1;
	}

	return 0;
}

bool Color4f::validChannel(float value) {
	return ((Float::cmp(value, 0.0f) >= 0) && (Float::cmp(value, 1.0f) <= 0));
}

int Color4f::_invalChannel(const char *channel, float value) {
	fprintf(stderr, "Color4f: Invalid %s channel value: %f (expected [0.0, 1.0])\n",
		channel, value);
	return -1;
}
