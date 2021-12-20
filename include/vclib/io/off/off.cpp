/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include "off.h"

#include "../internal/io_read.h"

namespace vcl::io::off {

static const float GEOMVIEW_COLOR_MAP[148][4] = {
	{1.0f, 1.0f, 1.0f, 1.0f},     {1.0f, 1.0f, 1.0f, 1.0f},     {1.0f, 1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},     {1.0f, 1.0f, 1.0f, 1.0f},     {1.0f, 1.0f, 1.0f, 1.0f},
	{0.7f, 0.7f, 0.7f, 0.7f},     {0.2f, 0.2f, 0.2f, 0.2f},     {0.9f, 0.9f, 0.9f, 0.9f},
	{0.1f, 0.1f, 0.1f, 0.1f},     {0.1f, 0.1f, 0.1f, 0.1f},     {0.8f, 0.8f, 0.8f, 0.8f},
	{0.7f, 0.7f, 0.7f, 0.7f},     {0.7f, 0.7f, 0.7f, 0.7f},     {0.0f, 0.0f, 0.0f, 0.0f},
	{0.9f, 0.9f, 0.9f, 0.9f},     {0.2f, 0.2f, 0.2f, 0.2f},     {0.0f, 0.0f, 0.0f, 0.0f},
	{0.75f, 0.75f, 0.75f, 0.75f}, {0.8f, 0.8f, 0.8f, 0.8f},     {0.8f, 0.8f, 0.8f, 0.8f},
	{0.0f, 0.0f, 0.0f, 0.0f},     {0.0f, 0.0f, 0.0f, 0.0f},     {0.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 0.0f},     {0.4f, 0.4f, 0.4f, 0.4f},     {0.4f, 0.4f, 0.4f, 0.4f},
	{0.8f, 0.8f, 0.8f, 0.8f},     {0.8f, 0.8f, 0.8f, 0.8f},     {0.7f, 0.7f, 0.7f, 0.7f},
	{0.7f, 0.7f, 0.7f, 0.7f},     {0.7f, 0.7f, 0.7f, 0.7f},     {0.7f, 0.7f, 0.7f, 0.7f},
	{0.0f, 0.0f, 0.0f, 0.0f},     {0.9f, 0.9f, 0.9f, 0.9f},     {0.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 0.0f},     {0.75f, 0.75f, 0.75f, 0.75f}, {0.8f, 0.8f, 0.8f, 0.8f},
	{0.4f, 0.4f, 0.4f, 0.4f},     {0.0f, 0.0f, 0.0f, 0.0f},     {0.0f, 0.0f, 0.0f, 0.0f},
	{0.4f, 0.4f, 0.4f, 0.4f},     {0.8f, 0.8f, 0.8f, 0.8f},     {0.7f, 0.7f, 0.7f, 0.7f},
	{0.7f, 0.7f, 0.7f, 0.7f},     {0.0f, 0.0f, 0.0f, 0.0f},     {0.9f, 0.9f, 0.9f, 0.9f},
	{0.0f, 0.0f, 0.0f, 0.0f},     {0.0f, 0.0f, 0.0f, 0.0f},     {0.75f, 0.75f, 0.75f, 0.75f},
	{0.8f, 0.8f, 0.8f, 0.8f},     {0.4f, 0.4f, 0.4f, 0.4f},     {0.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 0.0f},     {0.4f, 0.4f, 0.4f, 0.4f},     {0.8f, 0.8f, 0.8f, 0.8f},
	{0.7f, 0.7f, 0.7f, 0.7f},     {0.7f, 0.7f, 0.7f, 0.7f},     {0.0f, 0.0f, 0.0f, 0.0f},
	{0.9f, 0.9f, 0.9f, 0.9f},     {0.0f, 0.0f, 0.0f, 0.0f},     {0.0f, 0.0f, 0.0f, 0.0f},
	{0.75f, 0.75f, 0.75f, 0.75f}, {0.8f, 0.8f, 0.8f, 0.8f},     {0.4f, 0.4f, 0.4f, 0.4f},
	{0.0f, 0.0f, 0.0f, 0.0f},     {0.0f, 0.0f, 0.0f, 0.0f},     {0.4f, 0.4f, 0.4f, 0.4f},
	{0.8f, 0.8f, 0.8f, 0.8f},     {1.0f, 1.0f, 1.0f, 1.0f},     {1.0f, 1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},     {1.0f, 1.0f, 1.0f, 1.0f},     {1.0f, 1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f, 1.0f},     {0.05f, 0.05f, 0.05f, 0.05f}, {0.7f, 0.7f, 0.7f, 0.7f},
	{0.2f, 0.2f, 0.2f, 0.2f},     {0.9f, 0.9f, 0.9f, 0.9f},     {0.0f, 0.0f, 0.0f, 0.0f},
	{0.1f, 0.1f, 0.1f, 0.1f},     {0.8f, 0.8f, 0.8f, 0.8f},     {0.7f, 0.7f, 0.7f, 0.7f},
	{0.7f, 0.7f, 0.7f, 0.7f},     {0.7f, 0.7f, 0.7f, 0.7f},     {0.7f, 0.7f, 0.7f, 0.7f},
	{0.0f, 0.0f, 0.0f, 0.0f},     {0.0f, 0.0f, 0.0f, 0.0f},     {0.9f, 0.9f, 0.9f, 0.9f},
	{0.9f, 0.9f, 0.9f, 0.9f},     {0.0f, 0.0f, 0.0f, 0.0f},     {0.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 0.0f},     {0.0f, 0.0f, 0.0f, 0.0f},     {0.75f, 0.75f, 0.75f, 0.75f},
	{0.75f, 0.75f, 0.75f, 0.75f}, {0.8f, 0.8f, 0.8f, 0.8f},     {0.8f, 0.8f, 0.8f, 0.8f},
	{0.0f, 0.0f, 0.0f, 0.0f},     {0.0f, 0.0f, 0.0f, 0.0f},     {0.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 0.0f},     {0.4f, 0.4f, 0.4f, 0.4f},     {0.4f, 0.4f, 0.4f, 0.4f},
	{0.8f, 0.8f, 0.8f, 0.8f},     {0.8f, 0.8f, 0.8f, 0.8f},     {0.7f, 0.7f, 0.7f, 0.7f},
	{0.7f, 0.7f, 0.7f, 0.7f},     {0.7f, 0.7f, 0.7f, 0.7f},     {0.7f, 0.7f, 0.7f, 0.7f},
	{0.0f, 0.0f, 0.0f, 0.0f},     {0.9f, 0.9f, 0.9f, 0.9f},     {0.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 0.0f},     {0.75f, 0.75f, 0.75f, 0.75f}, {0.8f, 0.8f, 0.8f, 0.8f},
	{0.4f, 0.4f, 0.4f, 0.4f},     {0.0f, 0.0f, 0.0f, 0.0f},     {0.0f, 0.0f, 0.0f, 0.0f},
	{0.4f, 0.4f, 0.4f, 0.4f},     {0.8f, 0.8f, 0.8f, 0.8f},     {0.7f, 0.7f, 0.7f, 0.7f},
	{0.7f, 0.7f, 0.7f, 0.7f},     {0.0f, 0.0f, 0.0f, 0.0f},     {0.9f, 0.9f, 0.9f, 0.9f},
	{0.0f, 0.0f, 0.0f, 0.0f},     {0.0f, 0.0f, 0.0f, 0.0f},     {0.75f, 0.75f, 0.75f, 0.75f},
	{0.8f, 0.8f, 0.8f, 0.8f},     {0.4f, 0.4f, 0.4f, 0.4f},     {0.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 0.0f},     {0.4f, 0.4f, 0.4f, 0.4f},     {0.8f, 0.8f, 0.8f, 0.8f},
	{0.7f, 0.7f, 0.7f, 0.7f},     {0.7f, 0.7f, 0.7f, 0.7f},     {0.0f, 0.0f, 0.0f, 0.0f},
	{0.9f, 0.9f, 0.9f, 0.9f},     {0.0f, 0.0f, 0.0f, 0.0f},     {0.0f, 0.0f, 0.0f, 0.0f},
	{0.75f, 0.75f, 0.75f, 0.75f}, {0.8f, 0.8f, 0.8f, 0.8f},     {0.4f, 0.4f, 0.4f, 0.4f},
	{0.0f, 0.0f, 0.0f, 0.0f},     {0.0f, 0.0f, 0.0f, 0.0f},     {0.4f, 0.4f, 0.4f, 0.4f},
	{0.8f, 0.8f, 0.8f, 0.8f}};

void loadOffHeader(std::ifstream& file, FileMeshInfo& fileInfo, uint& nv, uint& nf, uint& ne)
{
	fileInfo.reset();
	vcl::Tokenizer           tokens = internal::nextNonEmptyTokenizedLine(file);
	vcl::Tokenizer::iterator token  = tokens.begin();
	std::string              header = *token;

	// the OFF string is in the header go on parsing it.
	if (header.rfind("OFF") != std::basic_string<char>::npos) {
		for (int u = header.rfind("OFF") - 1; u >= 0; u--) {
			if (header[u] == 'C')
				fileInfo.setVertexColors();
			else if (header[u] == 'N')
				fileInfo.setVertexNormals();
			else if (u > 0 && header[u - 1] == 'S' && header[u] == 'T')
				fileInfo.setVertexTexCoords();
			else if (header[u] == '4')
				throw vcl::MalformedFileException("Unsupported Homogeneus components in OFF.");
			else if (header[u] == 'n')
				throw vcl::MalformedFileException("Unsupported High Dimension OFF.");
		}
	}
	else
		throw vcl::MalformedFileException("Missing OFF header in file.");

	// If the file is slightly malformed and it has nvert and nface AFTER the OFF string instead of
	// in the next line we manage it here...
	if (tokens.size() == 1) {
		tokens = internal::nextNonEmptyTokenizedLine(file);
		token = tokens.begin();
	}
	else
		++token;

	nv = io::internal::readUInt<uint>(token);
	nf = io::internal::readUInt<uint>(token);
	ne = io::internal::readUInt<uint>(token);
	if (nv > 0)
		fileInfo.setVertices();
	if (nf > 0)
		fileInfo.setFaces();
	// if (ne > 0)
	//	loadedInfo.setEdges();
}

inline vcl::Color loadColor(vcl::Tokenizer::iterator& token, int nColorComponents)
{
	uint red, green, blue, alpha = 255;

	if (nColorComponents == 1) {
		uint k = io::internal::readUInt<uint>(token);

		red = GEOMVIEW_COLOR_MAP[k][0]*255;
		green = GEOMVIEW_COLOR_MAP[k][1]*255;
		blue = GEOMVIEW_COLOR_MAP[k][2]*255;
		alpha = GEOMVIEW_COLOR_MAP[k][3]*255;
	}
	else {
		double r = io::internal::readDouble<double>(token);
		double g = io::internal::readDouble<double>(token);
		double b = io::internal::readDouble<double>(token);
		double a = -1;
		if (nColorComponents == 4) {
			a = io::internal::readDouble<double>(token);
		}
		if (r > 1 || g > 1 || b > 1){
			red = r;
			green = g;
			blue = b;
			alpha = a != -1 ? a : alpha;
		}
		else {
			red = r * 255;
			green = g * 255;
			blue = b * 255;
			alpha = a != -1 ? a * 255 : alpha;
		}
	}
	return vcl::Color(red, green, blue, alpha);
}

} // namespace vcl::io::off
