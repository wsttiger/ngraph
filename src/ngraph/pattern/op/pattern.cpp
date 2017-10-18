// ----------------------------------------------------------------------------
// Copyright 2017 Nervana Systems Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// ----------------------------------------------------------------------------

#include "pattern.hpp"

void ngraph::pattern::op::Pattern::match_class(ngraph::pattern::Matcher& matcher, std::shared_ptr<Node> graph_node)
{
    bool is_match = true;
    if (is_binded())
    {
        if (get_binded_node() != graph_node)
        {
            is_match = false;
        }
    }
    else
    {
        m_binded = graph_node;
    }

    matcher.on_match_class(shared_from_this(),
        graph_node,
        is_match);

    //if subgraph underneath doesn't match unbind m_binded
    if (!matcher.is_match())
    {
        reset();
    }
}