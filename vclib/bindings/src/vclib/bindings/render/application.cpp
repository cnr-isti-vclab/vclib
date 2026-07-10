// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/render/application.h>
#include <vclib/render/application.h>

#include <pybind11/stl.h>

#include <memory>
#include <string>
#include <vector>

namespace vcl::bind {

struct ApplicationWrapper {
    int argc;
    std::vector<std::string> args_str;
    std::vector<char*> argv;
    std::unique_ptr<vcl::Application> app;

    ApplicationWrapper(const std::vector<std::string>& args)
    {
        argc     = args.size();
        args_str = args;
        for (auto& s : args_str) {
            argv.push_back(s.data());
        }
        app = std::make_unique<vcl::Application>(argc, argv.data());
    }

    ApplicationWrapper()
    {
        argc = 1;
        args_str.push_back("vclib_app");
        argv.push_back(args_str[0].data());
        app = std::make_unique<vcl::Application>(argc, argv.data());
    }

    int exec() { return app->exec(); }
};

void initApplication(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<ApplicationWrapper> c(m, "Application");

    c.def(py::init<const std::vector<std::string>&>(), py::arg("args"));
    c.def(py::init<>());

    c.def("exec", &ApplicationWrapper::exec);
}

} // namespace vcl::bind
