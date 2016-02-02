import os;
import os.path;

src_path = "."
dst_path = "../minimal_bs_shared_ptr"

from_namespace = "boost"
to_namespace = "bs"

from_shared_ptr = "shared_ptr"
to_shared_ptr = "ptr"

from_make_shared = "make_shared"
to_make_shared = "make"

from_enable_shared_from_this = "enable_shared_from_this"
to_enable_shared_from_this = "enable_ptr_from_this"

from_shared_from_this = "shared_from_this"
to_shared_from_this = "ptr_from_this"

from_static_pointer_cast = "static_pointer_cast"
to_static_pointer_cast = "static_ptr_cast"

from_const_pointer_cast = "const_pointer_cast"
to_const_pointer_cast = "const_ptr_cast"

from_dynamic_pointer_cast = "dynamic_pointer_cast"
to_dynamic_pointer_cast = "dynamic_ptr_cast"

from_reinterpret_pointer_cast = "reinterpret_pointer_cast"
to_reinterpret_pointer_cast = "reinterpret_ptr_cast"


def rename(src):
    dst = os.path.join(dst_path, src[len(src_path) + 1:])
    dst = dst.replace("/{0}/".format(from_namespace), "/{0}/".format(to_namespace))

    dstdir = os.path.dirname(dst)
    if not os.path.isdir(dstdir):
        os.makedirs(dstdir)

    fin = open(src)
    lines = fin.readlines()
    fin.close()

    fout = open(dst, "w")
    for line in lines:
        # namespace
        line = line.replace("<{0}/".format(from_namespace), "<{0}/".format(to_namespace))
        line = line.replace("namespace {0}".format(from_namespace), "namespace {0}".format(to_namespace))
        line = line.replace("{0}::".format(from_namespace), "{0}::".format(to_namespace))
        line = line.replace("{0}_".format(from_namespace.upper()), "{0}_".format(to_namespace.upper()))
        # shared_ptr
        line = line.replace("::{0}".format(from_shared_ptr), "::{0}".format(to_shared_ptr))
        line = line.replace(" {0}".format(from_shared_ptr), " {0}".format(to_shared_ptr))
        line = line.replace("({0}".format(from_shared_ptr), "({0}".format(to_shared_ptr))
        line = line.replace("<{0}".format(from_shared_ptr), "<{0}".format(to_shared_ptr))
        # make_shared
        line = line.replace("::{0}".format(from_make_shared), "::{0}".format(to_make_shared))
        line = line.replace(" {0}".format(from_make_shared), " {0}".format(to_make_shared))
        # enable_shared_from_this
        line = line.replace("::{0}".format(from_enable_shared_from_this), "::{0}".format(to_enable_shared_from_this))
        line = line.replace(" {0}".format(from_enable_shared_from_this), " {0}".format(to_enable_shared_from_this))
        line = line.replace("~{0}".format(from_enable_shared_from_this), "~{0}".format(to_enable_shared_from_this))
        line = line.replace("({0}".format(from_enable_shared_from_this), "({0}".format(to_enable_shared_from_this))
        # shared_from_this
        line = line.replace(" {0}".format(from_shared_from_this), " {0}".format(to_shared_from_this))
        line = line.replace(".{0}".format(from_shared_from_this), ".{0}".format(to_shared_from_this))
        line = line.replace("->{0}".format(from_shared_from_this), "->{0}".format(to_shared_from_this))
        # static_pointer_cast
        line = line.replace("{0}".format(from_static_pointer_cast), "{0}".format(to_static_pointer_cast))
        # const_pointer_cast
        line = line.replace("{0}".format(from_const_pointer_cast), "{0}".format(to_const_pointer_cast))
        # dynamic_pointer_cast
        line = line.replace("{0}".format(from_dynamic_pointer_cast), "{0}".format(to_dynamic_pointer_cast))
        # reinterpret_pointer_cast
        line = line.replace("{0}".format(from_reinterpret_pointer_cast), "{0}".format(to_reinterpret_pointer_cast))

        fout.write(line)
    fout.close()


def walkdir(path):
    for item in os.listdir(path):
        fullpath = os.path.join(path, item)
        if os.path.isdir(fullpath):
            if item.startswith("."):
                continue
            else:
                walkdir(os.path.join(path, item))
        else:
            if item.startswith("."):
                continue
            rename(os.path.join(path, item))

walkdir(src_path)
