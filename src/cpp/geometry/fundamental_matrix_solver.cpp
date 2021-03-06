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

#include "fundamental_matrix_solver.h"

#include <glog/logging.h>

namespace bsfm {

// Append two-view image match data to the list of image matches.
void FundamentalMatrixSolver::AddMatchedImagePair(
    const PairwiseImageMatch& matched_image_data) {
  matched_image_data_.push_back(matched_image_data);
}

// Append a set of data from two-view image matchesto the list of image matches.
void FundamentalMatrixSolver::AddMatchedImagePairs(
    const PairwiseImageMatchList& matched_image_data) {
  matched_image_data_.insert(matched_image_data_.end(),
                             matched_image_data.begin(),
                             matched_image_data.end());
}

void FundamentalMatrixSolver::SetOptions(
    const FundamentalMatrixSolverOptions& options) {
  options_ = options;
}

bool FundamentalMatrixSolver::ComputeFundamentalMatrices(
    std::vector<Matrix3d>& fundamental_matrices) {
  // Clear the output.
  fundamental_matrices.clear();

  // Determine a fundamental matrix for each pair of images.
  for (const auto& pair_data : matched_image_data_) {
    Matrix3d fundamental_matrix;
    if (ComputeFundamentalMatrix(pair_data.feature_matches_,
                                 fundamental_matrix)) {
      fundamental_matrices.push_back(fundamental_matrix);
    } else {
      VLOG(1) << "Failed to compute funamental matrix.";
    }
  }

  // Return whether or not we computed any fundamental matrices.
  if (fundamental_matrices.empty()) {
    VLOG(1) << "Unable to compute a fundamental matrix for any of the input "
               "image pairs.";
    return false;
  }

  return true;
}

}  //\namespace bsfm
