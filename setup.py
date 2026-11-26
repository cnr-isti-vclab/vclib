import os
import platform
import re
import shutil
import subprocess
import sys
import sysconfig
from setuptools import setup
from setuptools.command.build_ext import build_ext
from setuptools.command.install import install
from setuptools.command.sdist import sdist
from setuptools.command.bdist_wheel import bdist_wheel as _bdist_wheel

with open('VERSION', 'r') as f:
    vclib_version = f.read().strip()


def _parse_macos_arches(raw_arches):
    """Parse whitespace/semicolon/comma-separated macOS architecture names."""
    arches = []
    for token in re.split(r"[\s;,]+", raw_arches.strip()):
        if token in ("x86_64", "arm64") and token not in arches:
            arches.append(token)
    return arches


def _get_macos_target_arches(plat_name=None):
    """Resolve target macOS architectures from environment, plat_name, or host."""
    if os.environ.get("VCLIB_MACOS_UNIVERSAL", "").strip() == "1":
        return ["x86_64", "arm64"]

    cmake_osx_arches = os.environ.get("CMAKE_OSX_ARCHITECTURES", "")
    if cmake_osx_arches.strip():
        arches = _parse_macos_arches(cmake_osx_arches)
        if arches:
            return arches

    archflags = os.environ.get("ARCHFLAGS", "")
    if archflags.strip():
        arches = _parse_macos_arches(
            " ".join(re.findall(r"-arch\s+([^\s]+)", archflags))
        )
        if arches:
            return arches

    if plat_name:
        normalized = plat_name.lower().replace("-", "_")
        if normalized.endswith("_universal2") or normalized == "universal2":
            return ["x86_64", "arm64"]
        if normalized.endswith("_x86_64"):
            return ["x86_64"]
        if normalized.endswith("_arm64"):
            return ["arm64"]

    return ["arm64"] if platform.machine() == "arm64" else ["x86_64"]


def _get_macos_deployment_target():
    """Return explicit macOS deployment target from environment if configured."""
    target = (
        os.environ.get("MACOSX_DEPLOYMENT_TARGET")
        or os.environ.get("CMAKE_OSX_DEPLOYMENT_TARGET")
        or ""
    ).strip()
    return target or None


def _get_macos_cmake_args(plat_name=None):
    """Build macOS-specific CMake flags for cross or universal2 wheel builds."""
    args = []
    arches = _get_macos_target_arches(plat_name)
    deploy_target = _get_macos_deployment_target()

    explicit_arch = bool(
        os.environ.get("VCLIB_MACOS_UNIVERSAL", "").strip() == "1"
        or os.environ.get("CMAKE_OSX_ARCHITECTURES", "").strip()
        or os.environ.get("ARCHFLAGS", "").strip()
        or (plat_name and "universal2" in plat_name.lower())
    )
    if len(arches) > 1 or explicit_arch:
        args.append(f"-DCMAKE_OSX_ARCHITECTURES={';'.join(arches)}")
    if deploy_target:
        args.append(f"-DCMAKE_OSX_DEPLOYMENT_TARGET={deploy_target}")
    if len(arches) > 1:
        # Avoid linking single-arch Homebrew libraries into universal2 wheels.
        args.extend([
            "-DVCLIB_ALLOW_SYSTEM_TBB=OFF",
            "-DVCLIB_ALLOW_SYSTEM_GLFW=OFF",
            "-DVCLIB_ALLOW_SYSTEM_BGFX=OFF",
        ])
    return args


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
                arches = _get_macos_target_arches(getattr(self, 'plat_name', None))
                deploy_target = _get_macos_deployment_target() or '11.0'
                deploy_tag = deploy_target.replace('.', '_')
                if '_' not in deploy_tag:
                    deploy_tag += '_0'
                arch_tag = 'universal2' if set(arches) >= {'x86_64', 'arm64'} else arches[0]
                platform_tag = f'macosx_{deploy_tag}_{arch_tag}'
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
        build_temp = os.environ.get("VCLIB_WHEEL_BUILD_DIR", self.build_temp)
        build_temp = os.path.abspath(build_temp)
        os.makedirs(build_temp, exist_ok=True)

        cmake_args = [
            f"--preset=vclib-python-wheel",
            "-UPython*",
            f"-DPython_EXECUTABLE={sys.executable}"
        ]

        if platform.system() == "Darwin":
            plat_name = getattr(self.distribution, "_vclib_plat_name", None)
            cmake_args.extend(_get_macos_cmake_args(plat_name))

        if os.environ.get("VCLIB_USE_CCACHE") == "1":
            cmake_args.extend([
                "-DCMAKE_C_COMPILER_LAUNCHER=ccache",
                "-DCMAKE_CXX_COMPILER_LAUNCHER=ccache"
            ])

        build_args = ["--target", "install"]

        # Run CMake configure
        subprocess.check_call(["cmake", "-S", ".", "-B", build_temp] + cmake_args)

        # Run CMake build
        subprocess.check_call(["cmake", "--build", build_temp] + build_args)

        # Copy the built files to the build_lib directory
        dist_lib = os.path.join("dist", f"lib/python{sys.version_info.major}.{sys.version_info.minor}/site-packages/vclib")
        build_lib = os.path.join(self.build_lib, "vclib")
        if os.path.exists(dist_lib):
            if os.path.exists(build_lib):
                shutil.rmtree(build_lib)
            os.makedirs(os.path.dirname(build_lib), exist_ok=True)
            shutil.copytree(dist_lib, build_lib, dirs_exist_ok=True)

            if os.environ.get("VCLIB_LOCAL_WHEEL_BUILD", "").strip() == "1":
                init_file = os.path.join(build_lib, "__init__.py")
                if os.path.exists(init_file):
                    with open(init_file, "r") as f:
                        content = f.read()
                    
                    old_code = (
                        "    this_path = os.path.dirname(__file__)\n"
                        "    qt6_path = os.path.abspath(os.path.join(this_path, '..', 'PyQt6', 'Qt6', 'bin'))"
                    )
                    new_code = (
                        "    if 'Qt6_DIR' in os.environ:\n"
                        "        qt6_path = os.path.abspath(os.path.join(os.environ['Qt6_DIR'], 'bin'))\n"
                        "    else:\n"
                        "        this_path = os.path.dirname(__file__)\n"
                        "        qt6_path = os.path.abspath(os.path.join(this_path, '..', 'PyQt6', 'Qt6', 'bin'))"
                    )
                    content = content.replace(old_code, new_code)
                    
                    with open(init_file, "w") as f:
                        f.write(content)

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
        self.distribution._vclib_plat_name = getattr(self, "plat_name", None)
        self.run_command("build_ext")
        super().run()

inst_req = [
    "numpy>=2.0",
    "pyqt6==6.10.*",
    "msvc-runtime ; platform_system == 'Windows'",
]

setup(
    name="vclib",
    version=vclib_version,
    packages=['vclib'],
    author="Visual Computing Lab",
    author_email="vclab@isti.cnr.it",
    description="Visual Computing Library",
    long_description=open("README.md").read(),
    long_description_content_type="text/markdown",
    url="https://github.com/cnr-isti-vclab/vclib",
    include_package_data=True,
    package_data={
        "vclib": [],
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
        "Operating System :: OS Independent",
    ],
    zip_safe=False,
)
