/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_SELECTION_INFO_BGFX_H
#define VCL_SELECTION_INFO_BGFX_H

#include <vclib/render/selection/selection_mode.h>
#include <vclib/render/selection/selection_box.h>

namespace vcl {
    template<SelectionMode::Enum = SelectionMode::VERTEX_REGULAR>
    struct SelectionInfoBGFX {
        private:
            using ThisType = SelectionInfoBGFX<SelectionMode::VERTEX_REGULAR>;
        public:
        uint viewId;
        SelectionBox box;

        static ThisType build(uint viewId1, uint viewId2, SelectionBox box) {
            return ThisType{viewId1, box};
        }
    };

    template<SelectionMode::Enum = SelectionMode::VERTEX_ADD>
    struct SelectionInfoBGFX {
        private:
            using ThisType = SelectionInfoBGFX<SelectionMode::VERTEX_ADD>;
        public:
        uint viewId;
        SelectionBox box;

        static ThisType build(uint viewId1, uint viewId2, SelectionBox box) {
            return ThisType{viewId1, box};
        }
    };

    template<SelectionMode::Enum = SelectionMode::VERTEX_SUBTRACT>
    struct SelectionInfoBGFX {
        private:
            using ThisType = SelectionInfoBGFX<SelectionMode::VERTEX_SUBTRACT>;
        public:
        uint viewId;
        SelectionBox box;

        static ThisType build(uint viewId1, uint viewId2, SelectionBox box) {
            return ThisType{viewId1, box};
        }
    };

    template<SelectionMode::Enum = SelectionMode::VERTEX_INVERT>
    struct SelectionInfoBGFX {
        private:
            using ThisType = SelectionInfoBGFX<SelectionMode::VERTEX_INVERT>;
        public:
        uint viewId;

        static ThisType build(uint viewId1, uint viewId2, SelectionBox box) {
            return ThisType{viewId1};
        }
    };

    template<SelectionMode::Enum = SelectionMode::VERTEX_ALL>
    struct SelectionInfoBGFX {
        private:
            using ThisType = SelectionInfoBGFX<SelectionMode::VERTEX_ALL>;
        public:
        uint viewId;

        static ThisType build(uint viewId1, uint viewId2, SelectionBox box) {
            return ThisType{viewId1};
        }
    };

    template<SelectionMode::Enum = SelectionMode::VERTEX_NONE>
    struct SelectionInfoBGFX {
        private:
            using ThisType = SelectionInfoBGFX<SelectionMode::VERTEX_NONE>;
        public:
        uint viewId;

        static ThisType build(uint viewId1, uint viewId2, SelectionBox box) {
            return ThisType{viewId1};
        }
    };

    template<SelectionMode::Enum = SelectionMode::FACE_REGULAR>
    struct SelectionInfoBGFX {
        private:
            using ThisType = SelectionInfoBGFX<SelectionMode::FACE_REGULAR>;
        public:
        uint viewId;
        SelectionBox box;

        static ThisType build(uint viewId1, uint viewId2, SelectionBox box) {
            return ThisType{viewId1, box};
        }
    };

    template<SelectionMode::Enum = SelectionMode::FACE_ADD>
    struct SelectionInfoBGFX {
        private:
            using ThisType = SelectionInfoBGFX<SelectionMode::FACE_ADD>;
        public:
        uint viewId;
        SelectionBox box;

        static ThisType build(uint viewId1, uint viewId2, SelectionBox box) {
            return ThisType{viewId1, box};
        }
    };

    template<SelectionMode::Enum = SelectionMode::FACE_SUBTRACT>
    struct SelectionInfoBGFX {
        private:
            using ThisType = SelectionInfoBGFX<SelectionMode::FACE_SUBTRACT>;
        public:
        uint viewId;
        SelectionBox box;

        static ThisType build(uint viewId1, uint viewId2, SelectionBox box) {
            return ThisType{viewId1, box};
        }
    };

    template<SelectionMode::Enum = SelectionMode::FACE_INVERT>
    struct SelectionInfoBGFX {
        private:
            using ThisType = SelectionInfoBGFX<SelectionMode::FACE_INVERT>;
        public:
        uint viewId;

        static ThisType build(uint viewId1, uint viewId2, SelectionBox box) {
            return ThisType{viewId1};
        }
    };

    template<SelectionMode::Enum = SelectionMode::FACE_ALL>
    struct SelectionInfoBGFX {
        private:
            using ThisType = SelectionInfoBGFX<SelectionMode::FACE_ALL>;
        public:
        uint viewId;

        static ThisType build(uint viewId1, uint viewId2, SelectionBox box) {
            return ThisType{viewId1};
        }
    };

    template<SelectionMode::Enum = SelectionMode::FACE_NONE>
    struct SelectionInfoBGFX {
        private:
            using ThisType = SelectionInfoBGFX<SelectionMode::FACE_NONE>;
        public:
        uint viewId;

        static ThisType build(uint viewId1, uint viewId2, SelectionBox box) {
            return ThisType{viewId1};
        }
    };

    template<SelectionMode::Enum = SelectionMode::FACE_VISIBLE_REGULAR>
    struct SelectionInfoBGFX {
        private:
            using ThisType = SelectionInfoBGFX<SelectionMode::FACE_VISIBLE_REGULAR>;
        public:
        std::array<uint, 2> passViewIds;
        SelectionBox box;

        static ThisType build(uint viewId1, uint viewId2, SelectionBox box) {
            return ThisType{std::array<uint, 2>{viewId1, viewId2}, box};
        }
    };

    template<SelectionMode::Enum = SelectionMode::FACE_VISIBLE_ADD>
    struct SelectionInfoBGFX {
        private:
            using ThisType = SelectionInfoBGFX<SelectionMode::FACE_VISIBLE_REGULAR>;
        public:
        std::array<uint, 2> passViewIds;
        SelectionBox box;

        static ThisType build(uint viewId1, uint viewId2, SelectionBox box) {
            return ThisType{std::array<uint, 2>{viewId1, viewId2}, box};
        }
    };

    template<SelectionMode::Enum = SelectionMode::FACE_VISIBLE_SUBTRACT>
    struct SelectionInfoBGFX {
        private:
            using ThisType = SelectionInfoBGFX<SelectionMode::FACE_VISIBLE_SUBTRACT>;
        public:
        std::array<uint, 2> passViewIds;
        SelectionBox box;

        static ThisType build(uint viewId1, uint viewId2, SelectionBox box) {
            return ThisType{std::array<uint, 2>{viewId1, viewId2}, box};
        }
    };
}

#endif