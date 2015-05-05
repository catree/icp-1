//  This file is part of the Icp Library,
//
//  Copyright (C) 2014 by Arnaud TANGUY <arn.tanguy@NOSPAM.gmail.com>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.

#include <gtest/gtest.h>
#include <pcl/common/transforms.h>
#include <Eigen/Dense>
#include "eigentools.hpp"
#include "icp.hpp"
#include "error_point_to_point.hpp"
#include "logging.hpp"

namespace test_icp {

using namespace icp;

class TestErrorPointToPoint : public ::testing::Test
{
  protected:
    virtual void SetUp() {
      pc1_ = pcl::PointCloud<pcl::PointXYZ>::Ptr(
               new pcl::PointCloud<pcl::PointXYZ>());
      pc2_ = pcl::PointCloud<pcl::PointXYZ>::Ptr(
               new pcl::PointCloud<pcl::PointXYZ>());
      const int pointcloud_size = 100;
      err_expected_.resize(3 * pointcloud_size, 1);
      for (int i = 0; i < pointcloud_size; ++i)
      {
        pc1_->push_back(pcl::PointXYZ(i, 2.f * i, 3.f * i));
        pc2_->push_back(pcl::PointXYZ(-2.f * i, 3.f * i, i));
        err_expected_[i * 3] = 3.f * i;
        err_expected_[i * 3 + 1] =  -i;
        err_expected_[i * 3 + 2] = 2.f * i;
      }
    }

    virtual void TearDown() {
    }

    pcl::PointCloud<pcl::PointXYZ>::Ptr pc1_;
    pcl::PointCloud<pcl::PointXYZ>::Ptr pc2_;
    ErrorPointToPointXYZ err_;
    Eigen::Matrix<float, Eigen::Dynamic, 1> err_expected_;
};


TEST_F(TestErrorPointToPoint, IdentityErrorVector) {
  err_.setInputReference(pc1_);
  err_.setInputCurrent(pc1_);
  err_.computeError();

  Eigen::MatrixXf ev = err_.getErrorVector();
  Eigen::MatrixXf ev_expected = Eigen::MatrixXf::Zero(ev.rows(), 1);

  EXPECT_EQ(ev_expected, ev) <<
                             "Error vector should have been null as the two point clouds are identical!";
}

TEST_F(TestErrorPointToPoint, RealErrorVector) {
  err_.setInputReference(pc1_);
  err_.setInputCurrent(pc2_);
  err_.computeError();

  Eigen::MatrixXf ev = err_.getErrorVector();
  //LOG(INFO) << ev - err_expected_;

  // XXX: how good is the float comparison?
  EXPECT_TRUE(err_expected_.isApprox(ev))
             << "Expected:\n" << err_expected_ << "\nActual:\n" << ev;
}

/**
 * This test was generated by using Matlab to explicitely compute the jacobian
 * for two sample point clouds
 **/
TEST_F(TestErrorPointToPoint, JacobianUpdate) {
  auto pc1_m = pcl::PointCloud<pcl::PointXYZ>::Ptr(
                 new pcl::PointCloud<pcl::PointXYZ>());
  auto pc1_d = pcl::PointCloud<pcl::PointXYZ>::Ptr(
                 new pcl::PointCloud<pcl::PointXYZ>());
 pc1_m->push_back(pcl::PointXYZ(-0.046771458245186,  -0.146939507448636,   0.242446114493936));
 pc1_m->push_back(pcl::PointXYZ(-0.012488994722311,   0.019218224487079,   0.095940050940798));
 pc1_m->push_back(pcl::PointXYZ( 0.147895849150403,  -0.082229327629018,  -0.031577199500941));
 pc1_m->push_back(pcl::PointXYZ(-0.086081569000696,  -0.009424058795972,   0.042862267985940));
 pc1_m->push_back(pcl::PointXYZ( 0.078466846728782,   0.033621334095424,  -0.103598477851339));
 pc1_m->push_back(pcl::PointXYZ( 0.030862313994853,  -0.090465405924684,   0.187786546049586));
 pc1_m->push_back(pcl::PointXYZ(-0.023386004212915,  -0.028825636120570,   0.094070440335213));
 pc1_m->push_back(pcl::PointXYZ(-0.105697274596013,   0.035006275753418,   0.078734577993525));
 pc1_m->push_back(pcl::PointXYZ(-0.028414095462616,  -0.183585914250430,  -0.087587426195667));
 pc1_m->push_back(pcl::PointXYZ(-0.008669028245931,   0.103597590824548,   0.031994913438233));


pc1_d->push_back(pcl::PointXYZ(0.174628946840876,  -0.090747672451132,   0.239122394563635));
pc1_d->push_back(pcl::PointXYZ(0.079089337055351,   0.053245390513668,   0.096343241235499));
pc1_d->push_back(pcl::PointXYZ(0.262207005188406,   0.096963593220551,  -0.033395694725284));
pc1_d->push_back(pcl::PointXYZ(0.046474893091242,  -0.018210887363215,   0.042642246636861));
pc1_d->push_back(pcl::PointXYZ(0.130079399870358,   0.130883411889700,  -0.102826009061662));
pc1_d->push_back(pcl::PointXYZ(0.188730692463312,   0.004931356456402,   0.185730828231582));
pc1_d->push_back(pcl::PointXYZ(0.105323520344171,   0.011608886708395,   0.093406920445111));
pc1_d->push_back(pcl::PointXYZ(0.001756548545600,  -0.001241021176739,   0.079492571766547));
pc1_d->push_back(pcl::PointXYZ(0.208333745558328,  -0.098485486248070,  -0.091643689944723));
pc1_d->push_back(pcl::PointXYZ(0.021127860079352,   0.116594271343427,   0.034288131035024));

  Eigen::Matrix<float, 6, 1> expected_result;
  ///expected_result << 0.0008f, 0.0497f, -0.0025f, 0.0157f, 0.0157f, 0.0001f;
  expected_result <<  -0.142513353825048, 0.026363566665563, 0.004115131755677, -0.075215663389197, 0.014601072079337, -0.706267106105253;

  err_.setInputCurrent(pc1_m);
  err_.setInputReference(pc1_d);
  err_.computeError();
  err_.computeJacobian();

  Eigen::MatrixXf err_vector = err_.getErrorVector();
  Eigen::MatrixXf J = err_.getJacobian();
  //Eigen::MatrixXf result = (J.transpose()*J).inverse()*J.transpose()*err_vector;
  Eigen::MatrixXf result = eigentools::pseudoInverse(J) * err_vector;

  ASSERT_TRUE(expected_result.isApprox(result, 10e-3)) 
      << "\nExpected:\n" << expected_result << "\nActual:\n" << result;




  auto pc2_m = pcl::PointCloud<pcl::PointXYZ>::Ptr(
                 new pcl::PointCloud<pcl::PointXYZ>());
  auto pc2_d = pcl::PointCloud<pcl::PointXYZ>::Ptr(
                 new pcl::PointCloud<pcl::PointXYZ>());
 pc2_m->push_back(pcl::PointXYZ(-5.320113768088207 , -10.642134128893268,   3.918942094324491));
 pc2_m->push_back(pcl::PointXYZ( 16.821035946631788,  16.034572981200441, -12.506789068264075));
 pc2_m->push_back(pcl::PointXYZ( -8.757293461600174,  12.346791468907778,  -9.479609223314320));
 pc2_m->push_back(pcl::PointXYZ( -4.838150501101211,  -2.296264509631805,  -7.411060939404114));
 pc2_m->push_back(pcl::PointXYZ( -7.120045490274225, -15.061597039797190,  -5.078175502781736));
 pc2_m->push_back(pcl::PointXYZ(-11.742123314568163,  -4.446278164469854,  -3.205755066002393));
 pc2_m->push_back(pcl::PointXYZ( -1.922395175392748,  -1.559410357247689,   0.124690413616180));
 pc2_m->push_back(pcl::PointXYZ( -2.740702299326022,   2.760682539315362, -30.291773414041458));
 pc2_m->push_back(pcl::PointXYZ( 15.300725144240960,  -2.611636457764786,  -4.570146408715826));
 pc2_m->push_back(pcl::PointXYZ( -2.490247425137138,   4.434219129040914,  12.424484063907384));


 pc2_d->push_back(pcl::PointXYZ(-5.257901371853150 ,-10.653682285694934 ,  3.834370241928964));
 pc2_d->push_back(pcl::PointXYZ( 16.622512672723921,  16.283168787769000, -12.516024023559059));
 pc2_d->push_back(pcl::PointXYZ( -8.905112326101113,  12.541988538777543,  -9.145798826925830));
 pc2_d->push_back(pcl::PointXYZ( -4.953961516273003,  -2.130781367944225,  -7.369315649202131));
 pc2_d->push_back(pcl::PointXYZ( -7.198931621918938, -14.931740910632350,  -5.201676883417036));
 pc2_d->push_back(pcl::PointXYZ(-11.791028530000492,  -4.348279052662950,  -3.090388741434893));
 pc2_d->push_back(pcl::PointXYZ( -1.920199462378281,  -1.511650579918701,   0.130357442573114));
 pc2_d->push_back(pcl::PointXYZ( -3.216166682636981,   3.285409573570320, -30.197893146787369));
 pc2_d->push_back(pcl::PointXYZ( 15.227052791947951,  -2.485763390648605,  -4.850344361188038));
 pc2_d->push_back(pcl::PointXYZ( -2.294784895864792,   4.287926459241119,  12.530178665319109));


  Eigen::Matrix<float, 6, 1> expected_result2;
  expected_result2 << 0.000031103263367, -0.050288499760193, 0.002192279140143, -0.015684760231580, -0.015728590131416, -0.000120155047649 ;

  err_.setInputCurrent(pc2_m);
  err_.setInputReference(pc2_d);
  err_.computeError();
  err_.computeJacobian();

  err_vector = err_.getErrorVector();
  J = err_.getJacobian();
  result = eigentools::pseudoInverse(J) * err_vector;

  ASSERT_TRUE(expected_result2.isApprox(result)) 
      << "\nExpected:\n" << expected_result2 << "\nActual:\n" << result;

}


TEST_F(TestErrorPointToPoint, TestWeights) {
  pcl::PointCloud<pcl::PointXYZ>::Ptr pc_m = pcl::PointCloud<pcl::PointXYZ>::Ptr(
               new pcl::PointCloud<pcl::PointXYZ>());
  pcl::PointCloud<pcl::PointXYZ>::Ptr pc_d = pcl::PointCloud<pcl::PointXYZ>::Ptr(
               new pcl::PointCloud<pcl::PointXYZ>());
  const int NBPOINTS = 100;
  Eigen::MatrixXf weights(NBPOINTS, 3);
  Eigen::Matrix<float, Eigen::Dynamic, 1> err_expected(3*NBPOINTS, 1);

  for(int i=0; i<NBPOINTS; i++) {
    pc_m->push_back(pcl::PointXYZ(1.f, 1.f, 1.f));
    pc_d->push_back(pcl::PointXYZ(0.f, 0.f, 0.f));
    weights(i, 0) = i;
    weights(i, 1) = 2*i;
    weights(i, 2) = 3*i;

    err_expected(i*3) = i;
    err_expected(i*3+1) = 2*i;
    err_expected(i*3+2) = 3*i;
  }


  err_.setInputReference(pc_m);
  err_.setInputCurrent(pc_d);
  LOG(INFO) << "setting weights";
  err_.setWeights(weights);
  err_.computeError();
  Eigen::Matrix<float, Eigen::Dynamic, 1> err_vector = err_.getErrorVector();

  ASSERT_TRUE(err_expected.isApprox(err_vector)) << "Wrong result";
}

}  // namespace test_icp
