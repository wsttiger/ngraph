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

#include <fstream>

#include "dump_sorted.hpp"
#include "ngraph/ngraph.hpp"
#include "util.hpp"

using namespace ngraph;
using namespace std;

pass::DumpSorted::DumpSorted(const string& output_file)
    : m_output_file{output_file}
{
}

bool pass::DumpSorted::run_on_call_list(list<Node*>& nodes)
{
    ofstream out{m_output_file};
    if (out)
    {
        for (const Node* node : nodes)
        {
            out << node->get_node_id() << "(";
            vector<string> inputs;
            for (const descriptor::Input& input : node->get_inputs())
            {
                inputs.push_back(input.get_tensor().get_name());
            }
            out << join(inputs);
            out << ") -> ";

            vector<string> outputs;
            for (const descriptor::Output& output : node->get_outputs())
            {
                outputs.push_back(output.get_tensor().get_name());
            }
            out << join(outputs);
            out << "\n";

            for (const descriptor::Tensor* tensor : node->liveness_live_list)
            {
                out << "    ";
                if (contains(node->liveness_new_list, tensor))
                {
                    out << "N ";
                }
                else if (contains(node->liveness_free_list, tensor))
                {
                    out << "F ";
                }
                else
                {
                    out << "L ";
                }
                out << tensor->get_name() << "\n";
            }
        }
    }

    return false;
}