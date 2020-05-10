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