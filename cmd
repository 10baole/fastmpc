#include "fastmpc/abp/function/abp_compare.h"
#include "fastmpc/abp/function/abp_binary.h"
#include "fastmpc/abp/function/abp_unary.h"

namespace fastmpc::abp {

    namespace {
        auto msb(ABPBuilder &builder, OpHandle operand) {
            return builder.shift_right(builder.a2b(operand), 63);
        }

        auto less_b(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
            return msb(builder, subtract(builder, left, right));
        }

        auto greater_b(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
            return less_b(builder, right, left);
        }
    }

    auto equal(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
        auto not_less    = builder.not_b(less_b(builder, left, right));
        auto not_greater = builder.not_b(greater_b(builder, left, right));
        auto result_b    = builder.and_bb(not_less, not_greater);
        return builder.b2a(result_b, 0);
    }

    auto less(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
        return builder.b2a(less_b(builder, left, right), 0);
    }

    auto greater(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
        return builder.b2a(greater_b(builder, left, right), 0);
    }

    auto greater_equal(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
        auto not_less = builder.not_b(less_b(builder, left, right));
        return builder.b2a(not_less, 0);
    }

    auto select(ABPBuilder &builder, OpHandle which, OpHandle left, OpHandle right) -> OpHandle {
        auto diff       = subtract(builder, left, right);
        auto false_path = multiply(builder, diff, which);
        return add(builder, right, false_path);
    }

    auto max(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
        auto which = greater(builder, left, right);
        return select(builder, which, left, right);
    }
    
}

given this abp_compare.cc, when running cmake, i still got these problems:
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
