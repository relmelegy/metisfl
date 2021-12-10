// MIT License
//
// Copyright (c) 2021 Project Metis
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef PROJECTMETIS_RC_PROJECTMETIS_CONTROLLER_AGGREGATIONS_AGGREGATION_FUNCTION_H_
#define PROJECTMETIS_RC_PROJECTMETIS_CONTROLLER_AGGREGATIONS_AGGREGATION_FUNCTION_H_

#include <vector>
#include <utility>

#include "projectmetis/proto/model.pb.h"

namespace projectmetis::controller {

// Given that we need to define an interface, we basically need to provide the
// signature of pure virtual functions Recall that, a pure virtual function is
// a function that has to be assigned the value of 0.
class AggregationFunction {
 public:
  virtual ~AggregationFunction() = default;

  // Compute the community model given a sequence of federated models and their
  // respective scaling factors. Each federated model refers to the local
  // model of a learner and the associated scaling factor value refers to the
  // contribution value of the local model in the federation.
  virtual FederatedModel Aggregate(std::vector<std::pair<const Model*, double>>& pairs) = 0;

  virtual std::string name() = 0;
};

} // namespace projectmetis::controller

#endif //PROJECTMETIS_RC_PROJECTMETIS_CONTROLLER_AGGREGATIONS_AGGREGATION_FUNCTION_H_
