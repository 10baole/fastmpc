bao@icsr:~/fastmpc-1/build$ cmake ../
-- Could NOT find Terminfo (missing: Terminfo_LIBRARIES Terminfo_LINKABLE) 
-- Could NOT find ZLIB (missing: ZLIB_LIBRARY) (found version "1.2.11")
-- Could NOT find Terminfo (missing: Terminfo_LIBRARIES Terminfo_LINKABLE) 
-- Could NOT find ZLIB (missing: ZLIB_LIBRARY) (found version "1.2.11")
-- Building with -fPIC
CMake Warning at third_party/abseil-cpp-20240116.rc1/CMakeLists.txt:82 (message):
  A future Abseil release will default ABSL_PROPAGATE_CXX_STD to ON for CMake
  3.8 and up.  We recommend enabling this option to ensure your project still
  builds correctly.


-- Performing Test COMPILER_SUPPORT_std=cpp03
-- Performing Test COMPILER_SUPPORT_std=cpp03 - Success
-- Standard libraries to link to explicitly: none
-- Found unsuitable Qt version "" from NOTFOUND
-- Could NOT find CHOLMOD (missing: CHOLMOD_INCLUDES CHOLMOD_LIBRARIES) 
-- Could NOT find UMFPACK (missing: UMFPACK_INCLUDES UMFPACK_LIBRARIES) 
-- Could NOT find KLU (missing: KLU_INCLUDES KLU_LIBRARIES) 
-- Could NOT find SuperLU (missing: SUPERLU_INCLUDES SUPERLU_LIBRARIES SUPERLU_VERSION_OK) (Required is at least version "4.0")
-- Looking for Fortran sgemm
-- Looking for Fortran sgemm - found
-- A version of Pastix has been found but pastix_nompi.h does not exist in the include directory. Because Eigen tests require a version without MPI, we disable the Pastix backend.
-- Could NOT find SPQR (missing: SPQR_INCLUDES SPQR_LIBRARIES) 
-- Found unsuitable Qt version "" from NOTFOUND
-- Could NOT find GoogleHash (missing: GOOGLEHASH_INCLUDES GOOGLEHASH_COMPILE) 
-- Could NOT find Adolc (missing: ADOLC_INCLUDES ADOLC_LIBRARIES) 
-- Could NOT find MPFR (missing: MPFR_LIBRARIES) (Required is at least version "1.0.0")
CMake Warning at /usr/local/share/cmake-3.22/Modules/FindCUDA.cmake:1054 (message):
  Expecting to find librt for libcudart_static, but didn't find it.
Call Stack (most recent call first):
  third_party/eigen-3.4.0/unsupported/test/CMakeLists.txt:293 (find_package)


-- Found unsuitable Qt version "" from NOTFOUND
-- Qt4 not found, so disabling the mandelbrot and opengl demos
-- Could NOT find CHOLMOD (missing: CHOLMOD_INCLUDES CHOLMOD_LIBRARIES) 
-- Could NOT find UMFPACK (missing: UMFPACK_INCLUDES UMFPACK_LIBRARIES) 
-- Could NOT find KLU (missing: KLU_INCLUDES KLU_LIBRARIES) 
-- Could NOT find SuperLU (missing: SUPERLU_INCLUDES SUPERLU_LIBRARIES SUPERLU_VERSION_OK) (Required is at least version "4.0")
-- A version of Pastix has been found but pastix_nompi.h does not exist in the include directory. Because Eigen tests require a version without MPI, we disable the Pastix backend.
-- ************************************************************
-- ***    Eigen's unit tests configuration summary          ***
-- ************************************************************
-- 
-- Build type:        Debug
-- Build site:        icsr
-- Build string:      linux-5.4.0-215-generic-llvm-clang++-21.0.0-sse2-64bit
-- Enabled backends:  Boost.Multiprecision,  fftw, 
-- Disabled backends: CHOLMOD,  UMFPACK,  KLU,  SuperLU,  PaStiX,  METIS,  SPQR,  Qt4 support,  GoogleHash,  Adolc,  MPFR C++,  OpenGL, 
-- Default order:     Column-major
-- Maximal matrix/vector size: 320
-- SSE2:              Using architecture defaults
-- SSE3:              Using architecture defaults
-- SSSE3:             Using architecture defaults
-- SSE4.1:            Using architecture defaults
-- SSE4.2:            Using architecture defaults
-- AVX:               Using architecture defaults
-- AVX2:              Using architecture defaults
-- FMA:               Using architecture defaults
-- AVX512:            Using architecture defaults
-- AVX512DQ:          Using architecture defaults
-- Altivec:           Using architecture defaults
-- VSX:               Using architecture defaults
-- MIPS MSA:          Using architecture defaults
-- ARM NEON:          Using architecture defaults
-- ARMv8 NEON:        Using architecture defaults
-- S390X ZVECTOR:     Using architecture defaults
-- C++11:             OFF
-- SYCL:              OFF
-- CUDA:              OFF
-- HIP:               OFF
-- 
CXX:               /usr/local/bin/clang++
 CXX_FLAGS:         -O0 -ggdb -fno-exceptions -fPIC -fno-semantic-interposition -fvisibility-inlines-hidden -Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -Wimplicit-fallthrough -Wcovered-switch-default -Wno-noexcept-type -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Wsuggest-override -Wstring-conversion -Wmisleading-indentation -Wctad-maybe-unsupported -std=c++03 -pedantic -Wall -Wextra -Wundef -Wcast-align -Wchar-subscripts -Wnon-virtual-dtor -Wunused-local-typedefs -Wpointer-arith -Wwrite-strings -Wformat-security -Wshorten-64-to-32 -Wenum-conversion -Wc++11-extensions -Wdouble-promotion -Wshadow -Wno-psabi -Wno-variadic-macros -Wno-long-long -fno-check-new
 Sparse lib flags:   

-- ************************************************************
-- 
-- Configured Eigen 3.4.0
-- 
-- Available targets (use: make TARGET):
-- ---------+--------------------------------------------------------------
-- Target   |   Description
-- ---------+--------------------------------------------------------------
-- install  | Install Eigen. Headers will be installed to:
--          |     <CMAKE_INSTALL_PREFIX>/<INCLUDE_INSTALL_DIR>
--          |   Using the following values:
--          |     CMAKE_INSTALL_PREFIX: /usr/local
--          |     INCLUDE_INSTALL_DIR:  include/eigen3
--          |   Change the install location of Eigen headers using:
--          |     cmake . -DCMAKE_INSTALL_PREFIX=yourprefix
--          |   Or:
--          |     cmake . -DINCLUDE_INSTALL_DIR=yourdir
-- doc      | Generate the API documentation, requires Doxygen & LaTeX
-- check    | Build and run the unit-tests. Read this page:
--          |   http://eigen.tuxfamily.org/index.php?title=Tests
-- blas     | Build BLAS library (not the same thing as Eigen)
-- uninstall| Remove files installed by the install target
-- ---------+--------------------------------------------------------------
-- 
-- Version: 10.2.1
-- Build type: Debug
-- 
-- 26.1.0
-- Could NOT find ZLIB (missing: ZLIB_LIBRARY) (found version "1.2.11")
-- Building StableHLO as an external LLVM project
-- Could NOT find ZLIB (missing: ZLIB_LIBRARY) (found version "1.2.11")
-- Building with -fPIC
-- xtl v0.7.7
-- Building xtensor v0.24.7
-- Found xtl v0.7.7
-- Configuring done
CMake Error at CMakeLists.txt:29 (add_library):
  Target "fastmpc" links to target "ZLIB::ZLIB" but the target was not found.
  Perhaps a find_package() call is missing for an IMPORTED target, or an
  ALIAS target is missing?


CMake Error at CMakeLists.txt:29 (add_library):
  Target "fastmpc" links to target "Terminfo::terminfo" but the target was
  not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?


CMake Error at fastmpc/abp/low/CMakeLists.txt:1 (add_library):
  Target "abp_low" links to target "ZLIB::ZLIB" but the target was not found.
  Perhaps a find_package() call is missing for an IMPORTED target, or an
  ALIAS target is missing?


CMake Error at fastmpc/abp/low/CMakeLists.txt:1 (add_library):
  Target "abp_low" links to target "Terminfo::terminfo" but the target was
  not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?


CMake Error at fastmpc/runtime/CMakeLists.txt:58 (add_executable):
  Target "runtime_test" links to target "ZLIB::ZLIB" but the target was not
  found.  Perhaps a find_package() call is missing for an IMPORTED target, or
  an ALIAS target is missing?


CMake Error at fastmpc/runtime/CMakeLists.txt:58 (add_executable):
  Target "runtime_test" links to target "Terminfo::terminfo" but the target
  was not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "hlo_to_pphlo" links to target "ZLIB::ZLIB" but the target was not
  found.  Perhaps a find_package() call is missing for an IMPORTED target, or
  an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:606 (add_mlir_library)
  third_party/spu-release-0.7.0/libspu/compiler/passes/CMakeLists.txt:6 (add_mlir_conversion_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "hlo_to_pphlo" links to target "Terminfo::terminfo" but the target
  was not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:606 (add_mlir_library)
  third_party/spu-release-0.7.0/libspu/compiler/passes/CMakeLists.txt:6 (add_mlir_conversion_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "pphlo_dialect" links to target "ZLIB::ZLIB" but the target was not
  found.  Perhaps a find_package() call is missing for an IMPORTED target, or
  an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/spu-release-0.7.0/libspu/dialect/CMakeLists.txt:42 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "pphlo_dialect" links to target "Terminfo::terminfo" but the target
  was not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/spu-release-0.7.0/libspu/dialect/CMakeLists.txt:42 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloPortableApi" links to target "ZLIB::ZLIB" but the target
  was not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/api/CMakeLists.txt:15 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloPortableApi" links to target "Terminfo::terminfo" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/api/CMakeLists.txt:15 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloLinalgTransforms" links to target "ZLIB::ZLIB" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/conversions/linalg/transforms/CMakeLists.txt:5 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloLinalgTransforms" links to target "Terminfo::terminfo" but
  the target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/conversions/linalg/transforms/CMakeLists.txt:5 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloTOSATransforms" links to target "ZLIB::ZLIB" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/conversions/tosa/transforms/CMakeLists.txt:25 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloTOSATransforms" links to target "Terminfo::terminfo" but
  the target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/conversions/tosa/transforms/CMakeLists.txt:25 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloBase" links to target "ZLIB::ZLIB" but the target was not
  found.  Perhaps a find_package() call is missing for an IMPORTED target, or
  an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/dialect/CMakeLists.txt:22 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloBase" links to target "Terminfo::terminfo" but the target
  was not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/dialect/CMakeLists.txt:22 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloBroadcastUtils" links to target "ZLIB::ZLIB" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/dialect/CMakeLists.txt:37 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloBroadcastUtils" links to target "Terminfo::terminfo" but
  the target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/dialect/CMakeLists.txt:37 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "ChloOps" links to target "ZLIB::ZLIB" but the target was not found.
  Perhaps a find_package() call is missing for an IMPORTED target, or an
  ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/dialect/CMakeLists.txt:57 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "ChloOps" links to target "Terminfo::terminfo" but the target was
  not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/dialect/CMakeLists.txt:57 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloRegister" links to target "ZLIB::ZLIB" but the target was
  not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/dialect/CMakeLists.txt:82 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloRegister" links to target "Terminfo::terminfo" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/dialect/CMakeLists.txt:82 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloAssemblyFormat" links to target "ZLIB::ZLIB" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/dialect/CMakeLists.txt:97 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloAssemblyFormat" links to target "Terminfo::terminfo" but
  the target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/dialect/CMakeLists.txt:97 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloSerialization" links to target "ZLIB::ZLIB" but the target
  was not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/dialect/CMakeLists.txt:106 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloSerialization" links to target "Terminfo::terminfo" but
  the target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/dialect/CMakeLists.txt:106 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloTypeInference" links to target "ZLIB::ZLIB" but the target
  was not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/dialect/CMakeLists.txt:120 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloTypeInference" links to target "Terminfo::terminfo" but
  the target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/dialect/CMakeLists.txt:120 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloOps" links to target "ZLIB::ZLIB" but the target was not
  found.  Perhaps a find_package() call is missing for an IMPORTED target, or
  an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/dialect/CMakeLists.txt:141 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloOps" links to target "Terminfo::terminfo" but the target
  was not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/dialect/CMakeLists.txt:141 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "Version" links to target "ZLIB::ZLIB" but the target was not found.
  Perhaps a find_package() call is missing for an IMPORTED target, or an
  ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/dialect/CMakeLists.txt:169 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "Version" links to target "Terminfo::terminfo" but the target was
  not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/dialect/CMakeLists.txt:169 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "VhloOps" links to target "ZLIB::ZLIB" but the target was not found.
  Perhaps a find_package() call is missing for an IMPORTED target, or an
  ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/dialect/CMakeLists.txt:201 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "VhloOps" links to target "Terminfo::terminfo" but the target was
  not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/dialect/CMakeLists.txt:201 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "VhloTypes" links to target "ZLIB::ZLIB" but the target was not
  found.  Perhaps a find_package() call is missing for an IMPORTED target, or
  an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/dialect/CMakeLists.txt:219 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "VhloTypes" links to target "Terminfo::terminfo" but the target was
  not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/dialect/CMakeLists.txt:219 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "ChloCAPI" links to target "ZLIB::ZLIB" but the target was not
  found.  Perhaps a find_package() call is missing for an IMPORTED target, or
  an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:577 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/integrations/c/CMakeLists.txt:16 (add_mlir_public_c_api_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "ChloCAPI" links to target "Terminfo::terminfo" but the target was
  not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:577 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/integrations/c/CMakeLists.txt:16 (add_mlir_public_c_api_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloCAPI" links to target "ZLIB::ZLIB" but the target was not
  found.  Perhaps a find_package() call is missing for an IMPORTED target, or
  an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:577 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/integrations/c/CMakeLists.txt:25 (add_mlir_public_c_api_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloCAPI" links to target "Terminfo::terminfo" but the target
  was not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:577 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/integrations/c/CMakeLists.txt:25 (add_mlir_public_c_api_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "VhloCAPI" links to target "ZLIB::ZLIB" but the target was not
  found.  Perhaps a find_package() call is missing for an IMPORTED target, or
  an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:577 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/integrations/c/CMakeLists.txt:35 (add_mlir_public_c_api_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "VhloCAPI" links to target "Terminfo::terminfo" but the target was
  not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:577 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/integrations/c/CMakeLists.txt:35 (add_mlir_public_c_api_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceApi" links to target "ZLIB::ZLIB" but the target
  was not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:15 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceApi" links to target "Terminfo::terminfo" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:15 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceAxes" links to target "ZLIB::ZLIB" but the target
  was not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:32 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceAxes" links to target "Terminfo::terminfo" but
  the target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:32 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceConfiguration" links to target "ZLIB::ZLIB" but
  the target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:40 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceConfiguration" links to target
  "Terminfo::terminfo" but the target was not found.  Perhaps a
  find_package() call is missing for an IMPORTED target, or an ALIAS target
  is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:40 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceElement" links to target "ZLIB::ZLIB" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:51 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceElement" links to target "Terminfo::terminfo" but
  the target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:51 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceErrors" links to target "ZLIB::ZLIB" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:61 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceErrors" links to target "Terminfo::terminfo" but
  the target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:61 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceIndex" links to target "ZLIB::ZLIB" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:70 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceIndex" links to target "Terminfo::terminfo" but
  the target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:70 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "InterpreterOps" links to target "ZLIB::ZLIB" but the target was not
  found.  Perhaps a find_package() call is missing for an IMPORTED target, or
  an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:85 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "InterpreterOps" links to target "Terminfo::terminfo" but the target
  was not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:85 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceNumPy" links to target "ZLIB::ZLIB" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:101 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceNumPy" links to target "Terminfo::terminfo" but
  the target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:101 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceOps" links to target "ZLIB::ZLIB" but the target
  was not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:110 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceOps" links to target "Terminfo::terminfo" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:110 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceProcess" links to target "ZLIB::ZLIB" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:129 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceProcess" links to target "Terminfo::terminfo" but
  the target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:129 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceProcessGrid" links to target "ZLIB::ZLIB" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:138 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceProcessGrid" links to target "Terminfo::terminfo"
  but the target was not found.  Perhaps a find_package() call is missing for
  an IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:138 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceScope" links to target "ZLIB::ZLIB" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:148 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceScope" links to target "Terminfo::terminfo" but
  the target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:148 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceTensor" links to target "ZLIB::ZLIB" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:159 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceTensor" links to target "Terminfo::terminfo" but
  the target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:159 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceToken" links to target "ZLIB::ZLIB" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:171 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceToken" links to target "Terminfo::terminfo" but
  the target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:171 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceTypes" links to target "ZLIB::ZLIB" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:181 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceTypes" links to target "Terminfo::terminfo" but
  the target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:181 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceValue" links to target "ZLIB::ZLIB" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:189 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloReferenceValue" links to target "Terminfo::terminfo" but
  the target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/reference/CMakeLists.txt:189 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloTestUtils" links to target "ZLIB::ZLIB" but the target was
  not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/tests/CMakeLists.txt:35 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloTestUtils" links to target "Terminfo::terminfo" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  third_party/stablehlo-0.16.0/stablehlo/tests/CMakeLists.txt:35 (add_mlir_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "CheckOps" links to target "ZLIB::ZLIB" but the target was not
  found.  Perhaps a find_package() call is missing for an IMPORTED target, or
  an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/tests/CMakeLists.txt:59 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "CheckOps" links to target "Terminfo::terminfo" but the target was
  not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/tests/CMakeLists.txt:59 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:969 (add_executable):
  Target "stablehlo-opt" links to target "ZLIB::ZLIB" but the target was not
  found.  Perhaps a find_package() call is missing for an IMPORTED target, or
  an ALIAS target is missing?
Call Stack (most recent call first):
  third_party/stablehlo-0.16.0/stablehlo/tools/CMakeLists.txt:39 (add_llvm_executable)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:969 (add_executable):
  Target "stablehlo-opt" links to target "Terminfo::terminfo" but the target
  was not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  third_party/stablehlo-0.16.0/stablehlo/tools/CMakeLists.txt:39 (add_llvm_executable)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:969 (add_executable):
  Target "stablehlo-translate" links to target "ZLIB::ZLIB" but the target
  was not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  third_party/stablehlo-0.16.0/stablehlo/tools/CMakeLists.txt:46 (add_llvm_executable)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:969 (add_executable):
  Target "stablehlo-translate" links to target "Terminfo::terminfo" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  third_party/stablehlo-0.16.0/stablehlo/tools/CMakeLists.txt:46 (add_llvm_executable)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:969 (add_executable):
  Target "stablehlo-lsp-server" links to target "ZLIB::ZLIB" but the target
  was not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  third_party/stablehlo-0.16.0/stablehlo/tools/CMakeLists.txt:81 (add_llvm_executable)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:969 (add_executable):
  Target "stablehlo-lsp-server" links to target "Terminfo::terminfo" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  third_party/stablehlo-0.16.0/stablehlo/tools/CMakeLists.txt:81 (add_llvm_executable)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloPasses" links to target "ZLIB::ZLIB" but the target was
  not found.  Perhaps a find_package() call is missing for an IMPORTED
  target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/transforms/CMakeLists.txt:19 (add_mlir_dialect_library)


CMake Error at llvm-build/lib/cmake/llvm/AddLLVM.cmake:590 (add_library):
  Target "StablehloPasses" links to target "Terminfo::terminfo" but the
  target was not found.  Perhaps a find_package() call is missing for an
  IMPORTED target, or an ALIAS target is missing?
Call Stack (most recent call first):
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:321 (llvm_add_library)
  llvm-build/lib/cmake/mlir/AddMLIR.cmake:600 (add_mlir_library)
  third_party/stablehlo-0.16.0/stablehlo/transforms/CMakeLists.txt:19 (add_mlir_dialect_library)


-- Generating done
CMake Generate step failed.  Build files cannot be regenerated correctly.


mkdir build
cd build
cmake -DZLIB_LIBRARY=/usr/lib/x86_64-linux-gnu/libz.so \
      -DTerminfo_LIBRARIES=/usr/lib/x86_64-linux-gnu/libtinfo.so \
      ../
