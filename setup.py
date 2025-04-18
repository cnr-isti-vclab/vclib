import os
import platform
import shutil
import subprocess
import sys
import sysconfig
from setuptools import setup
from setuptools.command.build_ext import build_ext
from setuptools.command.install import install
from setuptools.command.sdist import sdist
from setuptools.command.bdist_wheel import bdist_wheel as _bdist_wheel

try:
    class bdist_wheel(_bdist_wheel):
        def finalize_options(self):
            _bdist_wheel.finalize_options(self)
            self.root_is_pure = False
        def get_tag(self):
            a, b, c = super().get_tag()
            if platform.system() == 'Windows':
                platform_tag = 'win_amd64'
            elif platform.system() == 'Linux':
                arch = sysconfig.get_platform().split('-')[1]
                glibc_version = os.popen('ldd --version').read().split('\n')[0].split()[-1]
                glibc_version = glibc_version.replace('.', '_')
                platform_tag = 'manylinux_' + glibc_version + '_' + arch
            elif platform.system() == 'Darwin':
                arch = platform.machine()
                pltf = 'macosx_11_0_x86_64'
                if arch == 'arm64':
                    pltf = 'macosx_11_0_arm64'
                platform_tag = pltf
            return a, b, platform_tag
except ImportError:
    bdist_wheel = None

class CMakeBuild(build_ext):
    def run(self):
        # Ensure CMake is installed
        try:
            subprocess.check_call(["cmake", "--version"])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " + ", ".join(e.name for e in self.extensions))

        # Configure and build the project
        build_temp = os.path.abspath(self.build_temp)
        os.makedirs(build_temp, exist_ok=True)

        cmake_args = [
            f"--preset=vclib-python-wheel",
            "-DPYTHON_EXECUTABLE=" + sys.executable,
        ]

        build_args = ["--target", "install"]

        # Run CMake configure
        subprocess.check_call(["cmake", "-S", ".", "-B", build_temp] + cmake_args)

        # Run CMake build
        subprocess.check_call(["cmake", "--build", build_temp] + build_args)

        # Copy the built files to the build_lib directory
        dist_lib = os.path.join("dist", f"lib/python{sys.version_info.major}.{sys.version_info.minor}/site-packages/vclib")
        build_lib = os.path.join(self.build_lib, "vclib")
        if os.path.exists(dist_lib):
            os.makedirs(os.path.dirname(build_lib), exist_ok=True)
            shutil.copytree(dist_lib, build_lib, dirs_exist_ok=True)

        # Ensure the copied files are included in the package
        self.distribution.package_data = {"vclib": ["*.so", "*.pyd", "*.dll", "__init__.py"]}

class CustomInstall(install):
    def run(self):
        self.run_command("build_ext")
        super().run()

class CustomSDist(sdist):
    def run(self):
        self.run_command("build_ext")
        super().run()

class CustomBDistWheel(bdist_wheel):
    def run(self):
        self.run_command("build_ext")
        super().run()

inst_req = [
    "pyqt6==6.8"
]

# if on windows, add msvc-runtime as dependency
osused = platform.system()
if osused == 'Windows':
    inst_req.append('msvc-runtime')

_package_data = ['dist/poba.odt']

setup(
    name="vclib",
    version="0.1.0",
    packages=['vclib'],
    author="Visual Computing Lab",
    author_email="vclab@isti.cnr.it",
    description="Visual Computing Library",
    long_description=open("README.md").read(),
    long_description_content_type="text/markdown",
    url="https://github.com/cnr-isti-vclab/vclib",
    include_package_data=True,
    package_data={
        "vclib": _package_data,  # Include built modules and __init__.py
    },
    install_requires= inst_req,
    python_requires=">=3.8",
    ext_modules=[],
    cmdclass={
        "build_ext": CMakeBuild,
        "install": CustomInstall,
        "sdist": CustomSDist,
        "bdist_wheel": CustomBDistWheel,
    },
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: Mozilla Public License 2.0 (MPL 2.0)",
        "Operating System :: OS Independent",
    ],
    zip_safe=False,
)