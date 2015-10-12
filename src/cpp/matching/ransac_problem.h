/*
 * Copyright (c) 2015, The Regents of the University of California (Regents).
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *
 *    3. Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Please contact the author(s) of this library if you have any questions.
 * Authors: Erik Nelson            ( eanelson@eecs.berkeley.edu )
 *          David Fridovich-Keil   ( dfk@eecs.berkeley.edu )
 */

///////////////////////////////////////////////////////////////////////////////
//
// This class defines an abstract base class for RANSAC problems. Derived
// classes will probide the specific details, data types, and model type that the
// Ransac class will use.
//
///////////////////////////////////////////////////////////////////////////////


#ifndef BSFM_MATCHING_RANSAC_PROBLEM_H
#define BSFM_MATCHING_RANSAC_PROBLEM_H

#include <vector>

#include "../util/disallow_copy_and_assign.h"

namespace bsfm {

// Derive from this class when defining a specific RANSAC problem!
class RansacDataElement {
 public:
  RansacDataElement() { }
  virtual ~RansacDataElement() { }
}; //\class RansacDataElement



// Derive from this class when defining a specific RANSAC problem!
class RansacModel {
 public:
  RansacModel() { }
  virtual ~RansacModel() { }

  // Define these remaining methods in a derived class!
  virtual double Error() const = delete;
  virtual bool IsGoodFit(RansacDataElement, double error_tolerance) const = delete;
}; //\class RansacModel



// Derive from this class when defining a specific RANSAC problem!
class RansacProblem {
 public:
  RansacProblem();
  virtual ~RansacProblem() { }

  static RansacModel NullModel();
  virtual void SetModel(const RansacModel& model);
  virtual bool SolutionFound();

  // Define these remaining methods in a derived class!
  virtual std::vector<RansacDataElement> SampleData() const = delete;
  virtual std::vector<RansacDataElement> UnsampledData() const = delete;
  virtual RansacModel FitModel(const std::vector<RansacDataElement>& data) =
      delete;

 protected:
  std::vector<RansacDataElement> data_;
  RansacModel model_;
  bool solution_found_;

 private:
  DISALLOW_COPY_AND_ASSIGN(RansacProblem)
};  //\class RansacProblem

// -------------------- Implementation -------------------- //

RansacProblem::RansacProblem() : solution_found_(false) {
  model_ = NullModel();
}

RansacModel RansacProblem::NullModel() {
  return RansacModel();
}

void RansacProblem::SetModel(const RansacModel& model) {
  model_ = model;
}

bool RansacProblem::SolutionFound() {
  return solution_found_;
}

}  //\namespace bsfm
#endif