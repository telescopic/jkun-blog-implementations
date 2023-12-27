load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Q: Why are we setting this up? 
# A: To get deterministic and reproducible builds as opposed to depending on the host
#    machine's configuration for compilation. 

# Point to LLVM toolchain release listed at: https://shorturl.at/rvCGX
# There are tradeoffs between using GCC, LLVM Clang, etc. Ideally, we should benchmark the performance
# of both compile time & binary execution time for all toolchains/compiler options.
# See possible compiler/toolchain options at: https://shorturl.at/cntyA
# TODO: Add option to run all toolchains/compiler in parallel and generate compiler vs runtime reports.
http_archive(
    name = "toolchains_llvm",
    sha256 = "b7cd301ef7b0ece28d20d3e778697a5e3b81828393150bed04838c0c52963a01",
    strip_prefix = "toolchains_llvm-0.10.3",
    canonical_id = "0.10.3",
    url = "https://github.com/grailbio/bazel-toolchain/releases/download/0.10.3/toolchains_llvm-0.10.3.tar.gz",
)

load("@toolchains_llvm//toolchain:deps.bzl", "bazel_toolchain_dependencies")

bazel_toolchain_dependencies()

load("@toolchains_llvm//toolchain:rules.bzl", "llvm_toolchain")

llvm_toolchain(
    name = "llvm_toolchain",
    llvm_version = "16.0.0",
)

load("@llvm_toolchain//:toolchains.bzl", "llvm_register_toolchains")

llvm_register_toolchains()

# Gtest, C++ Testing framework
http_archive(
  name = "com_google_googletest",
  urls = ["https://github.com/google/googletest/archive/5ab508a01f9eb089207ee87fd547d290da39d015.zip"],
  strip_prefix = "googletest-5ab508a01f9eb089207ee87fd547d290da39d015",
)

