#pragma once
#include <vclib/render/interfaces/drawable_object_i.h>
#include <vclib/bgfx/drawable/lines/lines_settings.h>
#include <vclib/bgfx/context.h>
#include <bgfx/bgfx.h>
namespace vcl::lines {
    class DrawableLines : public vcl::DrawableObjectI {

        public:

            static std::unique_ptr<DrawableLines> create(const std::vector<LinesVertex> &points, LinesTypes type = LinesTypes::CPU_GENERATED);
            
            DrawableLines() = default;

            virtual ~DrawableLines() = default;

            vcl::Box3d boundingBox() const override { 
                return vcl::Box3d(vcl::Point3d(-1,-1,-1), vcl::Point3d(1, 1, 1));
            }

            bool isVisible() const override { return mVisible; }

            void setVisibility(bool vis) override { mVisible = vis; }

            LinesSettings& getSettings() { return mSettings; }

            virtual void update(const std::vector<LinesVertex> &points) = 0;

        protected: 
            bool mVisible = true;
            LinesSettings mSettings;
    };
}
