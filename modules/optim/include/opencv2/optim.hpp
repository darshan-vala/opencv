/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2008-2012, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef __OPENCV_OPTIM_HPP__
#define __OPENCV_OPTIM_HPP__

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/core/mat.hpp"

//uncomment the next line to print the debug info
//#define ALEX_DEBUG

namespace cv{namespace optim
{
//! generic class for optimization algorithms */
class CV_EXPORTS Solver : public Algorithm /* Algorithm is the base OpenCV class */
{
    public:
    class CV_EXPORTS Function
    {
    public:
        virtual ~Function(){}
        virtual double calc(InputArray args) const = 0;
    };
    class CV_EXPORTS Constraints
    {
    public:
        virtual ~Constraints(){}
    };

    //! could be reused for all the generic algorithms like downhill simplex. Return value is the maximum value of a function*/
    virtual double solve(const Function& F,const Constraints& C, OutputArray result) const = 0;

    /*virtual void setTermCriteria(const TermCriteria& criteria) = 0;
    virtual TermCriteria getTermCriteria() = 0;*/

    // more detailed API to be defined later ...
};

class CV_EXPORTS LPSolver : public Solver
{
public:
    class CV_EXPORTS LPFunction:public Solver::Function
    {
        Mat z;
    public:
        //! Note, that this class is supposed to be immutable, so it's ok to make only a shallow copy of z_in.*/
        LPFunction(Mat z_in):z(z_in){}
        ~LPFunction(){};
        const Mat& getz()const{return z;}
        double calc(InputArray args)const;
    };

    //!This class represents constraints for linear problem. There are two matrix stored: m-by-n matrix A and n-by-1 column-vector b.
    //!What this represents is the set of constraints Ax\leq b and x\geq 0. It can be shown that any set of linear constraints can be converted
    //!this form and **we shall create various constructors for this class that will perform these conversions**.
    class CV_EXPORTS LPConstraints:public Solver::Constraints
    {
        Mat A,b;
    public:
        ~LPConstraints(){};
        //! Note, that this class is supposed to be immutable, so it's ok to make only a shallow copy of A_in and b_in.*/
        LPConstraints(Mat A_in, Mat b_in):A(A_in),b(b_in){}
        const Mat& getA()const{return A;}
        const Mat& getb()const{return b;}
    };

    LPSolver(){}
    double solve(const Function& F,const Constraints& C, OutputArray result)const;
};

//!the return codes for solveLP() function
enum
{
    SOLVELP_UNBOUNDED    = -2, //problem is unbounded (target function can achieve arbitrary high values)
    SOLVELP_UNFEASIBLE    = -1, //problem is unfeasible (there are no points that satisfy all the constraints imposed)
    SOLVELP_SINGLE    = 0, //there is only one maximum for target function
    SOLVELP_MULTI    = 1 //there are multiple maxima for target function - the arbitrary one is returned
};

CV_EXPORTS_W int solveLP(const Mat& Func, const Mat& Constr, Mat& z);
}}// cv

#endif
