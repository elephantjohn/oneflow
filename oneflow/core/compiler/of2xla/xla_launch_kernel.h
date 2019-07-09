#ifndef ONEFLOW_CORE_COMPILER_OF2XLA_XLA_LAUNCH_KERNEL_H_
#define ONEFLOW_CORE_COMPILER_OF2XLA_XLA_LAUNCH_KERNEL_H_

#include "oneflow/core/kernel/kernel.h"

namespace oneflow {

template <DeviceType device_type, typename T>
class XlaLaunchKernel : public KernelIf<device_type> {
 public:
  XlaLaunchKernel() = default;
  virtual ~XlaLaunchKernel() {}

 private:
  void ForwardDataContent(const KernelCtx&,
                          std::function<Blob*(const std::string&)>) const override;

  void BuildLocalExecutable(xla::LocalClient *client,
                            const std::vector<Blob *> &entry_blobs,
                            const std::vector<std::string> &entry_blob_names,
                            mola::CompilationResult *compile_result) const;

  void AsyncRunExecutable(xla::LocalClient *client,
                          xla::LocalExecutable *executable,
                          const std::vector<Blob *> &entry_blobs,
                          const std::vector<xla::Shape> &input_shapes,
                          std::vector<Blob *> &output_blobs,
                          const xla::Shape &output_shape) const;

  xla::LocalClient *GetOrCreateLocalClient(int intra_op_num_threads) const;
};

}  // namespace oneflow

#endif  // ONEFLOW_CORE_COMPILER_OF2XLA_XLA_LAUNCH_KERNEL_H_

