#pragma once

#include <vector>

namespace NOX {

class GLBuffer;
class GLRenderTargetBase;
class GLVertexArray;
struct VertexFormat;

struct GLState {
    std::vector<VertexFormat> vertexFormats;
    std::vector<std::unique_ptr<GLVertexArray>> vertexArrays;

    const GLRenderTargetBase *currentRenderTarget;
    uint32_t currentVertexArrayIndex;

    uint32_t indexType;
    uint32_t primitiveTopology;
};

} // namespace NOX
