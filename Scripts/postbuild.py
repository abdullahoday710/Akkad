import sys
import os
from shutil import copy
from shutil import copytree, ignore_patterns, copyfile, rmtree
from distutils.dir_util import copy_tree

from fnmatch import fnmatch, filter
from os.path import isdir, join
from shutil import copytree

def include_patterns(*patterns):
    """Factory function that can be used with copytree() ignore parameter.

    Arguments define a sequence of glob-style patterns
    that are used to specify what files to NOT ignore.
    Creates and returns a function that determines this for each directory
    in the file hierarchy rooted at the source directory when used with
    shutil.copytree().
    """
    def _ignore_patterns(path, names):
        keep = set(name for pattern in patterns
                            for name in filter(names, pattern))
        ignore = set(name for name in names
                        if name not in keep and not isdir(join(path, name)))
        return ignore
    return _ignore_patterns


workspace_location = sys.argv[1]
project_name = sys.argv[2]
project_output_dir = sys.argv[3]

game_assembly_include_path = workspace_location +"/bin/" + project_output_dir + "/"+ project_name+ "/GameAssembly/3rdparty/"
dependencies_src_path = workspace_location + "/3rdparty/"

game_assembly_dst_path = workspace_location + "/bin/" + project_output_dir + "/" + project_name + "/GameAssembly"

def copy_runtime_files():
    runtime_src_path = workspace_location + "/bin/" + project_output_dir + "/Runtime"
    runtime_dest_path = workspace_location + "/bin/" + project_output_dir + "/" + project_name + "/Runtime"

    if(os.path.isdir(runtime_dest_path)):
        rmtree(runtime_dest_path)
    copytree(runtime_src_path, runtime_dest_path, ignore=include_patterns('*.exe'))

    web_runtime_src_path = workspace_location + "/bin/" + "Debug-Web-x86" + "/Runtime"
    web_runtime_dest_path = workspace_location + "/bin/" + project_output_dir + "/" + project_name + "/Runtime/Web"

    if(os.path.isdir(web_runtime_dest_path)):
        rmtree(web_runtime_dest_path)
    copytree(web_runtime_src_path, web_runtime_dest_path, ignore=include_patterns('*.html', '*.js', '*.wasm'))


def copy_base_game_assembly_files():
    game_assembly_src_path = workspace_location + "/GameAssembly/src"

    copy_tree(game_assembly_src_path, game_assembly_dst_path + "/src")

    game_assembly_build_src_path = workspace_location + "/GameAssembly/Build/premake5.lua"
    copyfile(game_assembly_build_src_path, game_assembly_dst_path + "/premake5.lua")

def copy_akkad_include_files():
    akkad_src_path = workspace_location + "/Akkad/"
    akkad_dest_path = game_assembly_dst_path + "/Engine/Akkad/"
    if(os.path.isdir(akkad_dest_path)):
        rmtree(akkad_dest_path)
    copytree(akkad_src_path, akkad_dest_path, ignore=include_patterns('*.h', '*.cpp', '*.lua', '*.hpp'))

def copy_resource_files():
    res_src_path = workspace_location + "/" + project_name + "/res"
    res_dest_path = workspace_location + "/bin/" + project_output_dir + "/" + project_name + "/res"
    copy_tree(res_src_path, res_dest_path)

def copy_dependencies():
    #spdlog
    spdlog_src_path = os.path.join(dependencies_src_path, "spdlog/")
    spdlog_dest_path = os.path.join(game_assembly_include_path, "spdlog/")
    if(os.path.isdir(spdlog_dest_path)):
        rmtree(spdlog_dest_path)
    copytree(spdlog_src_path, spdlog_dest_path, ignore=include_patterns('*.h', '*.cpp', '*.lua', '*.hpp'))

    #box2d
    box2d_src_path = os.path.join(dependencies_src_path, "box2d/")
    box2d_dest_path = os.path.join(game_assembly_include_path, "box2d/")
    if(os.path.isdir(box2d_dest_path)):
        rmtree(box2d_dest_path)
    copytree(box2d_src_path, box2d_dest_path, ignore=include_patterns('*.h', '*.cpp', '*.lua', '*.hpp'))

    #glad
    glad_src_path = os.path.join(dependencies_src_path, "glad/")
    glad_dest_path = os.path.join(game_assembly_include_path, "glad/")
    if(os.path.isdir(glad_dest_path)):
        rmtree(glad_dest_path)
    copytree(glad_src_path, glad_dest_path, ignore=include_patterns('*.h', '*.c', '*.lua'))

    #imgui
    imgui_src_path = os.path.join(dependencies_src_path, "imgui/")
    imgui_dest_path = os.path.join(game_assembly_include_path, "imgui/")
    if(os.path.isdir(imgui_dest_path)):
        rmtree(imgui_dest_path)
    copytree(imgui_src_path, imgui_dest_path, ignore=include_patterns('*.h', '*.cpp', '*.lua', '*.hpp'))

    #SPIRV-CROSS
    spvcross_src_path = os.path.join(dependencies_src_path, "SPIRV-Cross/")
    spvcross_dest_path = os.path.join(game_assembly_include_path, "SPIRV-Cross/")
    if(os.path.isdir(spvcross_dest_path)):
        rmtree(spvcross_dest_path)
    copytree(spvcross_src_path, spvcross_dest_path, ignore=include_patterns('*.h', '*.cpp', '*.lua', '*.hpp'))

def copy_header_only_dependencies():

    # entt
    entt_src_path = os.path.join(dependencies_src_path, "entt/single_include")
    entt_dest_path = os.path.join(game_assembly_include_path, "entt/single_include")
    copy_tree(entt_src_path, entt_dest_path)

    # stb headers
    stb_src_path = os.path.join(dependencies_src_path, "stb/include")
    stb_dest_path = os.path.join(game_assembly_include_path, "stb/include")
    copy_tree(stb_src_path, stb_dest_path)

    # glm
    glm_src_path = os.path.join(dependencies_src_path, "glm/include")
    glm_dest_path = os.path.join(game_assembly_include_path, "glm/include")
    copy_tree(glm_src_path, glm_dest_path)

    # json
    json_src_path = os.path.join(dependencies_src_path, "json/include")
    json_dest_path = os.path.join(game_assembly_include_path, "json/include")
    copy_tree(json_src_path, json_dest_path)


if __name__ == "__main__":
    copy_base_game_assembly_files()
    copy_akkad_include_files()
    copy_resource_files()
    copy_dependencies()
    copy_header_only_dependencies()
    copy_runtime_files()










