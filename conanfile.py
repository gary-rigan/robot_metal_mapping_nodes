from conans import ConanFile
from conans.tools import load
import re
import os

class robot_metal_mapping_nodes(ConanFile):
    python_requires = (
        "cmake-build-helper/[>=0.1]@navi/develop",
        "qmake-build-helper/[>=0.1]@navi/develop",
    )
    name = "robot_metal_mapping_nodes"
    license = "None"
    author = "GEL"
    url = "www.robsys.com"
    description = "robsys robot_metal_mapping_nodes"
    topics = ("basic", "data")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": True}
    generators = "qmake","cmake","vscode"
    scm = {
        "type": "git",
        "url": "auto",
        "revision": "auto",
        "password": os.environ.get("SECRET", None)
    }
    revision_mode = "scm"
    requires = (
        ("robsys_datasets/[>=0.0.1]@navi/develop"),
        ("robsys_system/[>=0.0.1]@navi/develop"),
        ("robot_metal_ipc_msg_bridge/[>=0.0.1]@navi/develop"),
        ("transform_server/[>=0.0.1]@navi/develop"),
    )

    def set_version(self):
        content = load(os.path.join(self.recipe_folder, "version.txt"))
        self.version = content.strip()

    def build_with_cmake(self):
            
        cmake = self.python_requires["cmake-build-helper"].module.CMakeBuildEnvironment(
            self
        )

        args = []
        if self.settings.os == "Macos":
            args += ["-DMACOS=ON"]
        cmake.configure(self.source_folder, args)
        cmake.build()

    def build_with_qmake(self): 
        qmake = self.python_requires["qmake-build-helper"].module.QMakeBuildEnvironment(
            self)
        config_args = []

        if "ARCH_CHIP" in self.env:
            if "rk3588" == self.env["ARCH_CHIP"]:
                config_args += ["ARCH_CHIP=3"]
            elif "rk3399" == self.env["ARCH_CHIP"]:
                config_args += ["ARCH_CHIP=2"]
            elif "rk3288" == self.env["ARCH_CHIP"]:
                config_args += ["ARCH_CHIP=1"]


        config_args += ["DEFINES+=ROBSYS_VERSION="+str(self.version)]
        config_args += ["BUILD_GCC_VER=" +str(self.settings.compiler.version)]
        config_args += ["TARGET_ARCH=" +str(self.settings.arch)]
        config_args += ["CONAN_BUILD=1"]


        pro_filename = "{}/"+self.name+".pro"
        pro_file = pro_filename.format(self.source_folder)

        qmake.configure_legacy(pro_file, config_args)
        qmake.build()

    def build(self):
        self.build_with_qmake()

    def package(self):
        self.copy(self.name, dst="bin", keep_path=False)
        self.copy("config/*", dst="config/"+self.name, keep_path=False)

