#pragma once
#include "fastmpc/abp/dialect/abp_builder.h"
#include "fastmpc/abp/dialect/abp_context.h"
#include "fastmpc/abp/dialect/abp_ops.h"
#include "fastmpc/flux/dialect/flux_builder.h"
#include "fastmpc/flux/dialect/flux_ops.h"
#include "fastmpc/flux/low/3pc/3pc_value.h"
#include "fastmpc/ir_base/attribute.h"
#include <map>

using namespace std;

namespace fastmpc::flux::_3pc {

    struct ValueVisitor {
        virtual ~ValueVisitor() = default;
        virtual void visit(PlainValue  operand) = 0;
        virtual void visit(CipherValue operand) = 0;
    };

    class _3PCLower {

        public:
            _3PCLower(FluxBuilder &builder, const abp::ABPContext &abp_context): builder_(&builder), abp_context_(&abp_context) {}
            virtual ~_3PCLower() = default;
            void run();

            void operator()(abp::OpHandle handle, abp::InputOp op);
            void operator()(abp::OpHandle handle, abp::OutputOp op);

            void operator()(abp::OpHandle handle, abp::ConstantOp op);
            void operator()(abp::OpHandle handle, abp::NegateAOp op);
            void operator()(abp::OpHandle handle, abp::NegatePOp op);
            void operator()(abp::OpHandle handle, abp::InverseOp op);
            void operator()(abp::OpHandle handle, abp::NotBOp op);
            void operator()(abp::OpHandle handle, abp::BitReverseOp op);
            void operator()(abp::OpHandle handle, abp::ShiftRightOp op);

            void operator()(abp::OpHandle handle, abp::TruncateAOp op);
            void operator()(abp::OpHandle handle, abp::TruncatePOp op);

            void operator()(abp::OpHandle handle, abp::P2AOp op);
            
            void operator()(abp::OpHandle handle, abp::BroadcastOp op);
            void operator()(abp::OpHandle handle, abp::ReshapeOp op);
            void operator()(abp::OpHandle handle, abp::SliceOp op);
            void operator()(abp::OpHandle handle, abp::TransposeOp op);

            void operator()(abp::OpHandle handle, abp::AddAAOp op);
            void operator()(abp::OpHandle handle, abp::AddAPOp op);
            void operator()(abp::OpHandle handle, abp::AddPPOp op);            
            void operator()(abp::OpHandle handle, abp::MultiplyAAOp op);
            void operator()(abp::OpHandle handle, abp::MultiplyAPOp op);
            void operator()(abp::OpHandle handle, abp::MultiplyPPOp op);

            void operator()(abp::OpHandle handle, abp::ConcateOp op);

        private:
            virtual auto get_cipher_value(abp::OpHandle handle) -> CipherValue = 0;
            virtual auto get_plain_value(abp::OpHandle  handle) -> PlainValue  = 0;
            virtual void visit_value(abp::OpHandle      handle, ValueVisitor &visitor) = 0;
            virtual void set_value(abp::OpHandle handle, CipherValue value) = 0;
            virtual void set_value(abp::OpHandle handle, PlainValue  value) = 0;

            void push(abp::OpHandle handle, PlainValue value);
            void push(abp::OpHandle handle, CipherValue value);

        protected:
            vector <PlainValue> plain_values_;
            vector <CipherValue> cipher_values_;
            map <abp::OpHandle, Value> map_;

            auto push_shape(abp::OpHandle handle) -> ShapeHandle;
            FluxBuilder *const builder_;
            const abp::ABPContext *const abp_context_;
        };

}