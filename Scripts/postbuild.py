import sys
import os
from shutil import copy
from shutil import copytree, ignore_patterns
from distutils.dir_util import copy_tree
from shutil import copyfile

workspace_location = sys.argv[1]
project_name = sys.argv[2]
project_output_dir = sys.argv[3]

game_assembly_include_path = workspace_location +"/bin/" + project_output_dir + "/"+ project_name+ "/GameAssembly/include/"
dependencies_src_path = workspace_location + "/3rdparty/"

def include_patterns(*patterns):
    def _ignore_patterns(path, names):
        keep = set(name for pattern in patterns
                    for name in filter(names, pattern))
        ignore = set(name for name in names
                            if name not in keep and not isdir(join(path, name)))
        return ignore
        return _ignore_patterns

def copy_base_game_assembly_files():
    game_assembly_src_path = workspace_location + "/GameAssembly/src"
    game_assembly_dst_path = workspace_location + "/bin/" + project_output_dir + "/" + project_name + "/GameAssembly"
    copy_tree(game_assembly_src_path, game_assembly_dst_path + "/src")

    game_assembly_build_src_path = workspace_location + "/GameAssembly/Build/premake5.lua"
    copyfile(game_assembly_build_src_path, game_assembly_dst_path + "/premake5.lua")

def copy_akkad_include_files():
    akkad_src_path = workspace_location + "/Akkad/src/Akkad"
    akkad_dest_path = game_assembly_include_path + "/Akkad/Akkad"
    if(not os.path.isdir(akkad_dest_path)):
        copytree(akkad_src_path, akkad_dest_path, ignore=ignore_patterns('*.cpp'))

def copy_resource_files():
    res_src_path = workspace_location + "/" + project_name + "/res"
    res_dest_path = workspace_location + "/bin/" + project_output_dir + "/" + project_name + "/res"
    copy_tree(res_src_path, res_dest_path)

def copy_dependencies():
    glm_src_path = os.path.join(dependencies_src_path, "glm")
    glm_dest_path = os.path.join(game_assembly_include_path, "glm")
    copy_tree(glm_src_path, glm_dest_path)

    spdlog_src_path = os.path.join(dependencies_src_path, "spdlog/include")
    spdlog_dest_path = os.path.join(game_assembly_include_path, "spdlog/include")
    copy_tree(spdlog_src_path, spdlog_dest_path)

    box2d_src_path = os.path.join(dependencies_src_path, "box2d/include")
    box2d_dest_path = os.path.join(game_assembly_include_path, "box2d/include")
    copy_tree(box2d_src_path, box2d_dest_path)

    entt_src_path = os.path.join(dependencies_src_path, "entt/single_include")
    entt_dest_path = os.path.join(game_assembly_include_path, "entt/single_include")
    copy_tree(entt_src_path, entt_dest_path)



if __name__ == "__main__":
    copy_base_game_assembly_files()
    copy_akkad_include_files()
    copy_resource_files()
    copy_dependencies()










