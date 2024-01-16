#include <nox/pipeline_layout.h>

namespace nox {

class GLPipelineLayout final : public PipelineLayout {
  public:
    [[nodiscard]] static bool validateInput(const PipelineLayoutDescription &description);

    explicit GLPipelineLayout(PipelineLayoutDescription &description);

    void bind() const;

  private:
    DescriptionSetLayoutBindings m_bindings;
};

} // namespace nox
