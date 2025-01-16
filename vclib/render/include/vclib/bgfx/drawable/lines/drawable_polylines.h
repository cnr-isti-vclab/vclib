#pragma once
#include <vclib/render/interfaces/drawable_object_i.h>
#include <vclib/bgfx/drawable/lines/lines_settings.h>

#include <bgfx/bgfx.h>

namespace vcl::lines {
    class DrawablePolylines : public vcl::DrawableObjectI {

        public:
            static std::unique_ptr<DrawablePolylines> create(const std::vector<LinesVertex> &points, LinesTypes type = LinesTypes::CPU_GENERATED);
            
            DrawablePolylines() = default;

            DrawablePolylines(const std::string& vs_name, const std::string& fs_name);

            DrawablePolylines(const DrawablePolylines& other);

            DrawablePolylines(DrawablePolylines&& other);

            virtual ~DrawablePolylines();

            vcl::Box3d boundingBox() const override { 
                return vcl::Box3d(vcl::Point3d(-1,-1,-1), vcl::Point3d(1, 1, 1));
            }

            bool isVisible() const override { return mVisible; }

            void setVisibility(bool vis) override { mVisible = vis; }

            LinesSettings& getSettings() { return mSettings; }

            virtual void update(const std::vector<LinesVertex> &points) = 0;

        protected: 
            bool mVisible = true;
            
            LinesSettings               mSettings;
            bgfx::ProgramHandle         mLinesPH = BGFX_INVALID_HANDLE;
    };
}