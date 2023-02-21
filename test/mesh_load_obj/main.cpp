/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
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

#include <iostream>

#include <vclib/mesh.h>
#include <vclib/io/load_obj.h>

//template<typename T>
//concept MyConcept = std::is_integral<T>::value; //could be anything...

//template<typename T>
//class F {
//public:
//	void foo() requires MyConcept<F>;
//};

//template<typename T>
//void F<T>::foo() requires MyConcept<F<T>>
//{
//  //long implementation...
//}

//----//

//template<typename T>
//class F {
//public:
//	void foo() {};
//};

//template<MyConcept T>
//class F<T> {
//public:
//	void foo();
//};

//template<MyConcept T>
//void F<T>::foo()
//{
//}

int main()
{
	vcl::io::FileMeshInfo loadedInfo;
	vcl::TriMesh m = vcl::io::loadObj<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/TextureDouble.obj", loadedInfo);

	return 0;
}
