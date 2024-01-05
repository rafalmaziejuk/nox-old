#include <nox/pipeline_layout.h>

namespace nox {

class GLPipelineLayout final : public PipelineLayout {
  public:
    [[nodiscard]] static bool validateInput(const PipelineLayoutDescriptor &descriptor);

    explicit GLPipelineLayout(PipelineLayoutDescriptor &descriptor);

    void bind() const;

  private:
    DescriptorSetLayoutBindings m_bindings;
};

} // namespace nox
