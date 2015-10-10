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

#ifndef BSFM_POSE_H
#define BSFM_POSE_H

#include <glog/logging.h>
#include <Eigen/Dense>
#include <iostream>

namespace bsfm {

class Pose {

private:
  Eigen::Matrix4d Rt_; // 4x4 homogeneous Pose matrix
  Eigen::Vector3d aa_; // axis-angle representation

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  // Construct a new Pose from a rotation matrix and translation vector.
  Pose(Eigen::Matrix3d &, Eigen::Vector3d &);

  // Deep copy constructor.
  Pose(const Pose &);
  
  // Destroy this Pose.
  ~Pose();

  // Project a 3D point into this Pose.
  Eigen::Vector2d project(Eigen::Vector3d &);

  // Compose this Pose with the given pose so that both refer to the identity Pose as 
  // specified by the given Pose.
  void compose(Pose &);

  // Print to StdOut.
  void print();

  // Test if this pose (Rt_ only) is approximately equal to another pose.
  bool isApprox(Pose &);

  // Convert to axis-angle representation.
  Eigen::VectorXd toAxisAngle();

  // Convert to matrix representation.
  Eigen::Matrix4d fromAxisAngle();

};

}; // namespace bsfm

#endif
