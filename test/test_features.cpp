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

#include <image/image.h>
#include <matching/descriptor_extractor.h>
#include <matching/keypoint_detector.h>
#include <strings/join_filepath.h>

#include <gflags/gflags.h>
#include <gtest/gtest.h>

DEFINE_string(features_image_file, "lenna.png",
              "Name of the image used for testing.");

namespace bsfm {

class TestFeatures : public ::testing::Test {
 protected:
  const std::string test_image = strings::JoinFilepath(
      BSFM_TEST_DATA_DIR, FLAGS_features_image_file.c_str());

  const std::vector<std::string> detector_types = {
      "SIFT",  "SURF", "FAST",   "STAR",   "ORB",
      "BRISK", "MSER", "GFTT",   "HARRIS", "DENSE", "SIMPLEBLOB"
  };

  const std::vector<std::string> descriptor_types = {
    "SIFT",  "SURF", "BRIEF", "BRISK", "FREAK"
  };

  const std::vector<int> expected_keypoint_counts = {
      1098, 1462, 3603, 180, 500, 163, 230, 1000, 635, 7396, 1
  };

  const std::vector<int> expected_descriptor_counts = {
    1098, 1098, 1037, 1033, 1019
  };

  const std::vector<int> expected_descriptor_lengths = {
    128, 64, 32, 64, 64
  };
};  //\class TestFeatures

TEST_F(TestFeatures, TestDetectKeypoints) {
  // Load the test image.
  Image image(test_image.c_str());

  // Run through the different detector types, detecting keypoints and making
  // sure we get the right number.
  KeypointDetector detector;
  for (size_t ii = 0; ii < detector_types.size(); ++ii) {
    EXPECT_TRUE(detector.SetDetector(detector_types[ii]));

    // Detect keypoints.
    KeypointList keypoints;
    EXPECT_TRUE(detector.DetectKeypoints(image, keypoints));

    // Check for the right value.
    EXPECT_EQ(expected_keypoint_counts[ii], keypoints.size());
    LOG(INFO) << "Detected " << keypoints.size()
              << " keypoints using detector type " << detector_types[ii] << ".";
  }
}

TEST_F(TestFeatures, TestExtractDescriptors) {
  // Load the test image.
  Image image(test_image.c_str());

  // Detect keypoints in the image.
  KeypointDetector detector;
  detector.SetDetector("SIFT");

  KeypointList keypoints;
  detector.DetectKeypoints(image, keypoints);

  // Run through the different descriptor types, extracting descriptors for each
  // keypoint.
  DescriptorExtractor extractor;
  for (size_t ii = 0; ii < descriptor_types.size(); ++ii) {
    EXPECT_TRUE(extractor.SetDescriptor(descriptor_types[ii]));

    // Extract descriptors.
    DescriptorList descriptors;
    EXPECT_TRUE(extractor.ExtractDescriptors(image, keypoints, descriptors));

    // Descriptors should have the correct dimensions.
    EXPECT_EQ(expected_descriptor_counts[ii], descriptors.rows);
    LOG(INFO) << "Extracted " << descriptors.rows << " descriptors with "
              << descriptors.cols << " dimensions using descriptor type "
              << descriptor_types[ii] << ".";
  }
}

} //\namespace bsfm
