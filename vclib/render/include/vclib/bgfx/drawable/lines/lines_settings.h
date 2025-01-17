#pragma once
#include <iostream>
#include <assert.h>
#include <bgfx/bgfx.h>
#include <vclib/bgfx/drawable/lines/lines_utils.h>

namespace vcl::lines {

    enum Caps {
        BUTT_CAP            = 0x00000000, // No extra space on caps
        SQUARE_CAP          = 0x00000001, // Extra space with square shape
        ROUND_CAP           = 0x00000002, // Extra space with round shape
        TRIANGLE_CAP        = 0x00000003  // Extra space with triangle
    };

    enum Joins {
        ROUND_JOIN          = 0x00000000, // Join with round shape
        BEVEL_JOIN          = 0x00000001, // Join with square shape
        MITER_JOIN          = 0x00000002, // Join with a miter
    };

    enum ColorToUse {
        PER_VERTEX_COLOR    = 0x00000000, // Select color form vertex color
        PER_EDGE_COLOR      = 0x00000001, // Select color from edge buffer color
        GENERAL_COLOR       = 0x00000002  // Use general color in uniform data
    };

    class LinesSettings {
        private:
            uint8_t mThickness;
            uint8_t mAntialias;
            uint8_t mBorder;
            uint32_t mBorderColor;
            uint32_t mGeneralColor;
            uint8_t mMiterLimit;

            Caps mLeftCap;
            Caps mRigthCap;
            Joins mJoin;
            ColorToUse mColorToUse;

            bgfx::UniformHandle mDataUH;

        public:

            LinesSettings() : 
                mThickness(5),
                mAntialias(0),
                mBorder(0),
                mBorderColor(LinesVertex::COLOR(0, 0, 0, 1)),
                mGeneralColor(LinesVertex::COLOR(0, 0, 0, 1)),
                mMiterLimit(mThickness * 2),
                mLeftCap(Caps::ROUND_CAP),
                mRigthCap(Caps::ROUND_CAP),
                mJoin(Joins::ROUND_JOIN),
                mColorToUse(ColorToUse::GENERAL_COLOR)
            {
                mDataUH = bgfx::createUniform("u_data", bgfx::UniformType::Vec4);
            }

            ~LinesSettings() {
                if(bgfx::isValid(mDataUH))
                    bgfx::destroy(mDataUH);
            }

            Joins getJoin() const { return mJoin; }

            void setThickness(uint8_t thickness) { mThickness = thickness; }

            void setAntialias(uint8_t antialias) { mAntialias = antialias; }

            void setBorder(uint8_t border) { mBorder = border; }

            void setBorderColor(uint32_t borderColor) { mBorderColor = borderColor; }

            void setGeneralColor(uint32_t generalColor) { mGeneralColor = generalColor; }

            void setMiterLimit(uint8_t miterLimit) {
                if(miterLimit < mThickness)
                    assert((void("Miter limit must be greatest then thickness * 2"), false));
                mMiterLimit = miterLimit;
            }

            void setLeftCap(Caps cap) { mLeftCap = cap; }

            void setRigthCap(Caps cap) { mRigthCap = cap; }

            void setJoin(Joins join) { mJoin = join; }

            void setColorToUse(ColorToUse colorToUse) { mColorToUse = colorToUse; }

        public:
            void bindUniformLines() const {
                uint32_t thickness_antialias_border_caps_color = ( 
                    0                                             |
                    mThickness                             << 24 |
                    mAntialias                             << 16 |
                    mBorder                                << 8  |
                    static_cast<uint8_t>(mLeftCap)         << 4  |
                    static_cast<uint8_t>(mRigthCap)        << 2  |
                    static_cast<uint8_t>(mColorToUse)  
                );
                uint32_t data[] = {mGeneralColor, thickness_antialias_border_caps_color, mBorderColor, 0};
                bgfx::setUniform(mDataUH, data);
            }

            void bindUniformPolylines() const {
                uint32_t thickness_antialias_border_miterlimit = ( 
                    0                                       |
                    mThickness                       << 24 |
                    mAntialias                       << 16 |
                    mBorder                          << 8  |
                    mMiterLimit 
                );

                uint32_t caps_join_color = (                  
                   0                                          |
                   static_cast<uint8_t>(mLeftCap)       << 6 |
                   static_cast<uint8_t>(mRigthCap)      << 4 |
                   static_cast<uint8_t>(mJoin)          << 2 |
                   static_cast<uint8_t>(mColorToUse)
                );

                uint32_t data[] = {mGeneralColor, thickness_antialias_border_miterlimit, mBorderColor, caps_join_color};
                bgfx::setUniform(mDataUH, data);
            }
    };
}