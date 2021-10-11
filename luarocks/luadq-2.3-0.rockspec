package = "luadq"
version = "2.3-0"
source = {
   url = "luadq-2.3-0.tar.gz"
}
description = {
   homepage = "http://github.com/bobbens/libdq",
   maintainer = "Edgar Simo-Serra",
   license = "LGPLv3",
   summary = "Bindings to libdq, a library for manipulation dual quaternions.",
   detailed = [[
      This provides a solid implementation of dual
      quaternions which can be used to represent any
      rigid solid displacement. The main application
      is theoretical kinematics and robotics.
      ]]
}
dependencies = {
   "lua >= 5.1"
}
external_dependencies = {
   LIBDQ = {
      header = "dq/dq.h",
      library = "libdq.so"
   }
}
build = {
   type = "builtin",
   modules = {
      luadq = {
         sources = { "luadq.c" },
         libraries = { "dq" }
      }
   }
}

