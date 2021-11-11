workspace(name = "wonop")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

http_archive(
    name = "bazel_skylib",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.0.2/bazel-skylib-1.0.2.tar.gz",
        "https://github.com/bazelbuild/bazel-skylib/releases/download/1.0.2/bazel-skylib-1.0.2.tar.gz",
    ],
    sha256 = "97e70364e9249702246c0e9444bccdc4b847bed1eb03c5a3ece4f83dfe6abc44",
)

load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")
bazel_skylib_workspace()


# ================================================================
# Dotnet 
# ================================================================


# A newer version should be fine
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
git_repository(
  name = "io_bazel_rules_dotnet",
  remote = "https://github.com/bazelbuild/rules_dotnet",
  branch = "master",
)

load("@io_bazel_rules_dotnet//dotnet:deps.bzl", "dotnet_repositories")
dotnet_repositories()

load("@io_bazel_rules_dotnet//dotnet:defs.bzl", "dotnet_register_toolchains", "dotnet_repositories_nugets", "dotnet_nuget_new")
dotnet_register_toolchains()
dotnet_repositories_nugets()

# ================================================================
# LLVM
# ================================================================

# LLVM 13
LLVM_COMMIT = "09ac97ce350316b95b8cddb796d52f71b6f68296"

# The easiest way to calculate this for a new commit is to set it to empty and
# then run a bazel build and it will report the digest necessary to cache the
# archive and make the build reproducible.
LLVM_SHA256 = "2fb1aa06d12f8db349a27426cb0ced062987c5c2a75143c69f4284929e2750ff"

# FIXME: It shouldn't be necessary to use http_archive twice here. Caching
# should mean that this isn't too expensive though.

http_archive(
    name = "llvm-project-raw",
    build_file_content = "#empty",
    sha256 = LLVM_SHA256,
    strip_prefix = "llvm-project-" + LLVM_COMMIT,
    urls = ["https://github.com/llvm/llvm-project/archive/{commit}.tar.gz".format(commit = LLVM_COMMIT)],
)

http_archive(
    name = "llvm-bazel",
    sha256 = LLVM_SHA256,
    strip_prefix = "llvm-project-{}/utils/bazel".format(LLVM_COMMIT),
    urls = ["https://github.com/llvm/llvm-project/archive/{commit}.tar.gz".format(commit = LLVM_COMMIT)],
)

load("@llvm-bazel//:configure.bzl", "llvm_configure", "llvm_disable_optional_support_deps")

llvm_configure(
    name = "llvm-project",
    src_path = ".",
    src_workspace = "@llvm-project-raw//:WORKSPACE",
)

# Disables optional dependencies for Support like zlib and terminfo. You may
# instead want to configure them using the macros in the corresponding bzl
# files.
llvm_disable_optional_support_deps()

# ================================================================
# PyBind
# ================================================================

http_archive(
  name = "pybind11_bazel",
  sha256= "f1044df0475bbe819e285785ee9599d94f98ac3c86ddfb73fe16cfeb568bb381",  
  strip_prefix = "pybind11_bazel-16ed1b8f308d2b3dec9d7e6decaad49ce4d28b43",
  urls = ["https://github.com/pybind/pybind11_bazel/archive/16ed1b8f308d2b3dec9d7e6decaad49ce4d28b43.zip"],
)
http_archive(
  name = "pybind11",
  sha256 = "1859f121837f6c41b0c6223d617b85a63f2f72132bae3135a2aa290582d61520",
  build_file = "@pybind11_bazel//:pybind11.BUILD",
  strip_prefix = "pybind11-2.5.0",
  urls = ["https://github.com/pybind/pybind11/archive/v2.5.0.zip"],
)
load("@pybind11_bazel//:python_configure.bzl", "python_configure")
python_configure(name = "local_config_python")



# ================================================================
# Rust
# ================================================================

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "rules_rust",
    sha256 = "531bdd470728b61ce41cf7604dc4f9a115983e455d46ac1d0c1632f613ab9fc3",
    strip_prefix = "rules_rust-d8238877c0e552639d3e057aadd6bfcf37592408",
    urls = [
        # `main` branch as of 2021-08-23
        "https://github.com/bazelbuild/rules_rust/archive/d8238877c0e552639d3e057aadd6bfcf37592408.tar.gz",
    ],
)

load("@rules_rust//rust:repositories.bzl", "rust_repositories")

rust_repositories()




# ================================================================
# Google test
# ================================================================


http_archive(
    name = "googletest",
    url = "https://github.com/google/googletest/archive/release-1.10.0.zip",
    sha256 = "94c634d499558a76fa649edb13721dce6e98fb1e7018dfaeba3cd7a083945e91",
    strip_prefix = "googletest-release-1.10.0",
    build_file = "@//:config/BUILD.googletest",
)


# ================================================================
# libPFM - required by LLVM Jit
# ================================================================

# http_archive(
#     name = "libpfm4",
#     build_file = "@//:BUILD.libpfm4",
#     sha256 = "9193787a73201b4254e3669243fd71d15a9550486920861912090a09f366cf68",
#     strip_prefix = "libpfm-4.8.0",
#     urls = ["https://iweb.dl.sourceforge.net/project/perfmon2/libpfm4/libpfm-4.8.0.tar.gz"],
# )