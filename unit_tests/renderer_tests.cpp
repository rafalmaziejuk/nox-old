#include <nox/renderer.h>

#include <gtest/gtest.h>

using namespace NOX;

class RendererTestFixture : public ::testing::TestWithParam<RendererBackend> {
  public:
    void SetUp() override {
        renderer = Renderer::create(GetParam());
        ASSERT_NE(renderer, nullptr);
    }

    RendererPtr renderer{nullptr};
};

TEST_P(RendererTestFixture, GivenRendererBackendWhenCallingCreateRendererThenRendererWithCorrectRendererBackendIsCreated) {
    const auto expectedRendererBackend = GetParam();

    const auto rendererBackend = renderer->getRendererBackend();
    EXPECT_EQ(expectedRendererBackend, rendererBackend);
}

INSTANTIATE_TEST_SUITE_P(RendererTests,
                         RendererTestFixture,
                         ::testing::Values(RendererBackend::OPENGL));
