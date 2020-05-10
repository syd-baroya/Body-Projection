// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "RendererBase.h"

using namespace Visualization;

void RendererBase::UpdateViewProjection(
    glm::mat4 view,
    glm::mat4 projection)
{
    m_view = view;
    m_projection = projection;
}

inline void RendererBase::mat4_dup(glm::mat4 M, glm::mat4 N)
{
    int i, j;
    for (i = 0; i < 4; ++i)
        for (j = 0; j < 4; ++j)
            M[i][j] = N[i][j];
}
