#include "oneflow/core/framework/framework.h"
#include "oneflow/core/kernel/new_kernel_util.h"

namespace oneflow {

namespace {

template<DeviceType device_type>
class IdentityKernel final : public user_op::OpKernel {
 public:
  IdentityKernel() = default;
  ~IdentityKernel() override = default;

 private:
  void Compute(user_op::KernelComputeContext* ctx) const override {
    const user_op::Tensor* in = ctx->Tensor4ArgNameAndIndex("in", 0);
    user_op::Tensor* out = ctx->Tensor4ArgNameAndIndex("out", 0);
    const ShapeView& in_shape = in->shape();
    CHECK_EQ(out->shape(), in_shape);
    const DataType in_data_type = in->data_type();
    CHECK_EQ(out->data_type(), in_data_type);
    Memcpy<device_type>(ctx->device_ctx(), out->mut_dptr<void>(), in->dptr<void>(),
                        in_shape.elem_cnt() * GetSizeOfDataType(in_data_type));
  }
  bool AlwaysComputeWhenAllOutputsEmpty() const override { return false; }
};

#define REGISTER_IDENTITY_KERNEL(device)                                                        \
  REGISTER_USER_KERNEL("identity")                                                              \
      .SetCreateFn<IdentityKernel<device>>()                                                    \
      .SetIsMatchedHob(user_op::HobDeviceType() == device)                                      \
      .SetInplaceProposalFn([](const user_op::InferContext&,                                    \
                               user_op::AddInplaceArgPair AddInplaceArgPairFn) -> Maybe<void> { \
        OF_RETURN_IF_ERROR(AddInplaceArgPairFn("out", 0, "in", 0, false));                      \
        return Maybe<void>::Ok();                                                               \
      });

REGISTER_IDENTITY_KERNEL(DeviceType::kCPU)
REGISTER_IDENTITY_KERNEL(DeviceType::kGPU)

}  // namespace

}  // namespace oneflow
