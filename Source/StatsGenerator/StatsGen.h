/* ============================================================================
 * Copyright (c) 2010, Michael A. Jackson (BlueQuartz Software)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of Michael A. Jackson nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#ifndef _STATSGEN_H_
#define _STATSGEN_H_

#if defined (_MSC_VER)
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#endif

#include "AIM/Common/AIMMath.h"
#include "AIM/Common/Constants.h"
#include "AIM/Common/AIMRandomNG.h"

#include "AIM/Common/Texture.h"
#include "MXA/Common/LogTime.h"

/**
 * @class StatsGen StatsGen.h AIM/StatsGenerator/StatsGen.h
 * @author Michael A. Groeber US Air Force Research Laboratory
 * @date Dec 16, 2010
 * @version 1.0
 */
class StatsGen
{
  public:
    StatsGen();
    virtual ~StatsGen();

    /**
     * @brief Generates  XY Scatter plot data for a Beta Distribution function
     * @param alpha Alpha Term in the Beta Distribution function
     * @param beta Beta term in the Beta Distribution function
     * @param x X Values (Output)
     * @param y Y Value (Output)
     * @param size Number of points in the scatter plot
     * @return Error Code. 0 is NO ERROR.
     */
    template<typename T>
    int GenBetaPlotData(double alpha, double beta, T &x, T &y, int size)
    {
      int err = 0;
      double value, gammapq, gammap, gammaq, betain, betaout;
      x.resize(size);
      y.resize(size);
      value = alpha;
      gammap = gamma(value);
      value = beta;
      gammaq = gamma(value);
      value = alpha + beta;
      gammapq = gamma(value);
      for (int i = 0; i < size; i++)
      {
        betain = (i * (1.0 / double(size))) + ((1.0 / double(size)) / 2.0);
        betaout = (gammapq / (gammap * gammaq)) * pow(betain, (alpha - 1)) * pow((1 - betain), (beta - 1));
        x[i] = betain;
        y[i] = betaout * (1.0 / double(size));
        if (betaout < 0)
          err = 1;
      }
      return err;
    }


	template<typename T>
    int GenLogNormalPlotData(double avg, double stdDev, T &x, T &y, int size)
    {
      int err = 0;
      double lognormin, lognormout, max, min;
      double s2 = pow(stdDev, 2);
      double root2pi = pow((2.0 * 3.1415926535897), 0.5);
      x.resize(size);
      y.resize(size);
      min = exp(avg - (5 * stdDev));
      max = exp(avg + (5 * stdDev));
      for (int i = 0; i < size; i++)
      {
        lognormin = (i * ((max - min) / double(size))) + (((max - min) / double(size)) / 2.0) + min;
        lognormout = (1.0 / (lognormin * stdDev * root2pi)) * exp(-((log(lognormin) - avg) * (log(lognormin) - avg)) / (2 * s2));
        x[i] = lognormin;
        y[i] = lognormout * ((max - min) / double(size));
        if (lognormout < 0)
          err = 1;
      }
      return err;
    }

    template<typename T>
    int GenPowerLawPlotData(double alpha, double k, double beta, T &x, T &y, int size)
    {
      int err = 0;
      double in, out, max, min;
      x.resize(size);
      y.resize(size);
      min = 0;
      max = 5;
      for (int i = 0; i < size; i++)
      {
        in = (i * ((max - min) / double(size))) + (((max - min) / double(size)) / 2.0) + min;
        out = alpha * pow(in, k) + beta;
        x[i] = in;
        y[i] = out;
        if (out < 0)
          err = 1;
      }
      return err;
    }

    /**
     * @brief Calculates the number of bins using 64 bit floating point values
     * @param mu
     * @param sigma
     * @param cutoff
     * @param binstep
     * @param max (out)
     * @param min (out)
     * @return
     */
    int computeNumberOfBins(double mu, double sigma, double cutoff, double binstep, double &max, double &min);

    /**
     * @brief Calculates the number of bins using 32 bit floating point values
     * @param mu
     * @param sigma
     * @param cutoff
     * @param binstep
     * @param max (out)
     * @param min (out)
     * @return
     */
    int computeNumberOfBins(float mu, float sigma, float cutoff, float binstep, float &max, float &min);

    /**
     * @brief Generates the CutOff values
     * @param mu
     * @param sigma
     * @param cutoff
     * @param binstep
     * @param x
     * @param y
     * @param yMax
     * @param numsizebins
     * @param binsizes
     * @return
     */
    template<typename J, typename T>
    int GenCutOff(J mu, J sigma, J cutoff, J binstep, T &x, T &y, J yMax, int &numsizebins, T &binsizes)
    {
      J max, min;
      numsizebins = computeNumberOfBins(mu, sigma, cutoff, binstep, max, min);
      int err = 0;
      x.resize(2);
      y.resize(2);

      x[0] = min;
      x[1] = max;

      y[0] = 0.0;
      y[1] = yMax;

      binsizes.resize(numsizebins);
      for (int i = 0; i < numsizebins; i++)
      {
        binsizes[i] = min + (i * binstep);
      }
      return err;
    }

    /**
     * @brief
     * @param value
     * @return
     */
    double gamma(double value);

    /**
      * @brief  This method will generate ODF data for 3 scatter plots which are the
      * <001>, <011> and <111> directions. The method is a C++ Template which the parameter
      * type is a std::vector conforming class type that holds the data.
      * QVector falls into this category. The input data for the
      * euler angles is in Columnar fashion instead of row major format.
      * @param e1s The first euler angles (input)
      * @param e2s The second euler angles (input)
      * @param e3s The third euler angles (input)
      * @param weights Array of weights values. (input)
      * @param sigmas Array of sigma values. (input)
      * @param x001 X Values of the [001] PF Scatter plot (Output)
      * @param y001 Y Values of the [001] PF Scatter plot (Output)
      * @param x011 X Values of the [011] PF Scatter plot (Output)
      * @param y011 Y Values of the [011] PF Scatter plot (Output)
      * @param x111 X Values of the [111] PF Scatter plot (Output)
      * @param y111 Y Values of the [111] PF Scatter plot (Output)
      * @param size The number of points for the Scatter Plot
      */
       template<typename T>
       int GenCubicODFPlotData(T e1s, T e2s, T e3s, T weights, T sigmas,
                       T &x001, T &y001, T &x011, T &y011, T &x111, T &y111,
                       int size)
       {
         static const size_t eighteenCubed = 5832;
         double totalweight = 0;
         T odf;
         odf.resize(eighteenCubed);
         Texture::calculateCubicODFData(e1s, e2s, e3s, weights, sigmas, true, odf, totalweight);

         AIMRandomNG rg;
         /* Get a seed value based off the system clock. The issue is that this will
          * be a 64 bit unsigned integer where the high 32 bits will basically not
          * change where as the lower 32 bits will. The following lines of code will
          * pull off the low 32 bits from the number. This operation depends on most
          * significant byte ordering which is different between Big Endian and
          * Little Endian machines. For Big endian machines the Most Significant Byte
          * (MSB) is the first 32 bits. For Little Endian machines the MSB is the
          * second 32 bits.
          */
         unsigned long long int seed = MXA::getMilliSeconds();
         unsigned int* seedPtr = reinterpret_cast<unsigned int*> (&seed);
   #if CMP_WORDS_BIGENDIAN
         rg.RandomInit(seedPtr[1]);
   #else
         rg.RandomInit(seedPtr[0]);
   #endif
         int err = 0;
         int choose;
         double g[3][3];
         double x, y, z;
         double xpf, ypf;
         double xpfa, ypfa;
         double totaldensity;
         double hmag;
         double angle;
         double r1, r2, r3;
         double h1, h2, h3;
         double n1, n2, n3;
         double random, tan_angle, density, cos_angle, sin_angle;

         double dim1 = 2 * pow((0.75 * ((M_PI / 4.0) - sin((M_PI / 4.0)))), (1.0 / 3.0));
         double dim2 = 2 * pow((0.75 * ((M_PI / 4.0) - sin((M_PI / 4.0)))), (1.0 / 3.0));
         double dim3 = 2 * pow((0.75 * ((M_PI / 4.0) - sin((M_PI / 4.0)))), (1.0 / 3.0));

         x001.resize(size * 3);
         y001.resize(size * 3);
         x011.resize(size * 6);
         y011.resize(size * 6);
         x111.resize(size * 4);
         y111.resize(size * 4);

         for (int i = 0; i < size; i++)
         {
           random = rg.Random();
           choose = 0;

           totaldensity = 0;
           for (size_t j = 0; j < eighteenCubed; j++)
           {
             density = odf[j];
             totaldensity = totaldensity + density;
             if (random < totaldensity && random >= (totaldensity - density)) choose = static_cast<int> (j);
           }
           h1 = choose % 18;
           h2 = (choose / 18) % 18;
           h3 = choose / (18 * 18);
           random = rg.Random();
           h1 = ((dim1 / 18.0) * h1) + ((dim1 / 18.0) * random) - (dim1 / 2.0);
           random = rg.Random();
           h2 = ((dim2 / 18.0) * h2) + ((dim2 / 18.0) * random) - (dim2 / 2.0);
           random = rg.Random();
           h3 = ((dim3 / 18.0) * h3) + ((dim3 / 18.0) * random) - (dim3 / 2.0);
           hmag = pow((h1 * h1 + h2 * h2 + h3 * h3), 0.5);
           angle = pow((8 * hmag * hmag * hmag), (1.0 / 3.0));
           tan_angle = tan(angle / 2.0);
           n1 = h1 / hmag;
           n2 = h2 / hmag;
           n3 = h3 / hmag;
           r1 = tan_angle * n1;
           r2 = tan_angle * n2;
           r3 = tan_angle * n3;
           cos_angle = cos(angle);
           sin_angle = sin(angle);
           g[0][0] = cos_angle + n1 * n1 * (1 - cos_angle);
           g[0][1] = n1 * n2 * (1 - cos_angle) - n3 * sin_angle;
           g[0][2] = n1 * n3 * (1 - cos_angle) + n2 * sin_angle;
           g[1][0] = n1 * n2 * (1 - cos_angle) + n3 * sin_angle;
           g[1][1] = cos_angle + n2 * n2 * (1 - cos_angle);
           g[1][2] = n2 * n3 * (1 - cos_angle) - n1 * sin_angle;
           g[2][0] = n1 * n3 * (1 - cos_angle) - n2 * sin_angle;
           g[2][1] = n2 * n3 * (1 - cos_angle) + n1 * sin_angle;
           g[2][2] = cos_angle + n3 * n3 * (1 - cos_angle);
           x = g[0][0];
           y = g[1][0];
           z = g[2][0];
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           random = rg.Random();
           x001[3 * i] = xpf;
           y001[3 * i] = ypf;
           x = g[0][1];
           y = g[1][1];
           z = g[2][1];
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x001[3 * i + 1] = xpf;
           y001[3 * i + 1] = ypf;
           x = g[0][2];
           y = g[1][2];
           z = g[2][2];
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x001[3 * i + 2] = xpf;
           y001[3 * i + 2] = ypf;
           x = 0.707107 * (g[0][0] + g[0][1]);
           y = 0.707107 * (g[1][0] + g[1][1]);
           z = 0.707107 * (g[2][0] + g[2][1]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x011[6 * i] = xpf;
           y011[6 * i] = ypf;
           x = 0.707107 * (g[0][1] + g[0][2]);
           y = 0.707107 * (g[1][1] + g[1][2]);
           z = 0.707107 * (g[2][1] + g[2][2]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x011[6 * i + 1] = xpf;
           y011[6 * i + 1] = ypf;
           x = 0.707107 * (g[0][2] + g[0][0]);
           y = 0.707107 * (g[1][2] + g[1][0]);
           z = 0.707107 * (g[2][2] + g[2][0]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x011[6 * i + 2] = xpf;
           y011[6 * i + 2] = ypf;
           x = 0.707107 * (g[0][0] - g[0][1]);
           y = 0.707107 * (g[1][0] - g[1][1]);
           z = 0.707107 * (g[2][0] - g[2][1]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x011[6 * i + 3] = xpf;
           y011[6 * i + 3] = ypf;
           x = 0.707107 * (g[0][1] - g[0][2]);
           y = 0.707107 * (g[1][1] - g[1][2]);
           z = 0.707107 * (g[2][1] - g[2][2]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x011[6 * i + 4] = xpf;
           y011[6 * i + 4] = ypf;
           x = 0.707107 * (g[0][2] - g[0][0]);
           y = 0.707107 * (g[1][2] - g[1][0]);
           z = 0.707107 * (g[2][2] - g[2][0]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x011[6 * i + 5] = xpf;
           y011[6 * i + 5] = ypf;
           x = 0.57735 * (g[0][0] + g[0][1] + g[0][2]);
           y = 0.57735 * (g[1][0] + g[1][1] + g[1][2]);
           z = 0.57735 * (g[2][0] + g[2][1] + g[2][2]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x111[4 * i] = xpf;
           y111[4 * i] = ypf;
           x = 0.57735 * (g[0][0] + g[0][1] - g[0][2]);
           y = 0.57735 * (g[1][0] + g[1][1] - g[1][2]);
           z = 0.57735 * (g[2][0] + g[2][1] - g[2][2]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x111[4 * i + 1] = xpf;
           y111[4 * i + 1] = ypf;
           x = 0.57735 * (g[0][0] - g[0][1] + g[0][2]);
           y = 0.57735 * (g[1][0] - g[1][1] + g[1][2]);
           z = 0.57735 * (g[2][0] - g[2][1] + g[2][2]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x111[4 * i + 2] = xpf;
           y111[4 * i + 2] = ypf;
           x = 0.57735 * (-g[0][0] + g[0][1] + g[0][2]);
           y = 0.57735 * (-g[1][0] + g[1][1] + g[1][2]);
           z = 0.57735 * (-g[2][0] + g[2][1] + g[2][2]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x111[4 * i + 3] = xpf;
           y111[4 * i + 3] = ypf;
         }
         return err;
       }

       /**
        * @brief  This method will generate ODF data for a Hexagonal material and
        * generate 3 scatter plots which are the
        * <001>, <011> and <111> directions. The method is a C++ Template which the parameter
        * type is a std::vector conforming class type that holds the data.
        * QVector falls into this category. The input data for the
        * euler angles is in Columnar fashion instead of row major format.
        * @param e1s The first euler angles (input)
        * @param e2s The second euler angles (input)
        * @param e3s The third euler angles (input)
        * @param weights Array of weights values. (input)
        * @param sigmas Array of sigma values. (input)
        * @param x0001 X Values of the [0001] PF Scatter plot (Output)
        * @param y0001 Y Values of the [0001] PF Scatter plot (Output)
        * @param x1120 X Values of the [1120] PF Scatter plot (Output)
        * @param y1120 Y Values of the [1120] PF Scatter plot (Output)
        * @param x1010 X Values of the [1010] PF Scatter plot (Output)
        * @param y1010 Y Values of the [1010] PF Scatter plot (Output)
        * @param size The number of points for the Scatter Plot
        */
       template<typename T>
       int GenHexODFPlotData(T e1s, T e2s, T e3s, T weights, T sigmas,
                       T &x0001, T &y0001, T &x1120, T &y1120, T &x1010, T &y1010,
                       int size)
       {
         static const size_t odfsize = 15552;
         double totalweight = 0;
         T odf;
         odf.resize(odfsize);
         Texture::calculateHexODFData(e1s, e2s, e3s, weights, sigmas, true, odf, totalweight);

         AIMRandomNG rg;
         /* Get a seed value based off the system clock. The issue is that this will
          * be a 64 bit unsigned integer where the high 32 bits will basically not
          * change where as the lower 32 bits will. The following lines of code will
          * pull off the low 32 bits from the number. This operation depends on most
          * significant byte ordering which is different between Big Endian and
          * Little Endian machines. For Big endian machines the Most Significant Byte
          * (MSB) is the first 32 bits. For Little Endian machines the MSB is the
          * second 32 bits.
          */
         unsigned long long int seed = MXA::getMilliSeconds();
         unsigned int* seedPtr = reinterpret_cast<unsigned int*>(&seed);
   #if CMP_WORDS_BIGENDIAN
         rg.RandomInit(seedPtr[1]);
   #else
         rg.RandomInit(seedPtr[0]);
   #endif
         int err = 0;
         int choose;
         double g[3][3];
         double x, y, z;
         double xpf, ypf;
         double xpfa, ypfa;
         double totaldensity;
         double hmag;
         double angle;
         double r1, r2, r3;
         double h1, h2, h3;
         double n1, n2, n3;
         double random, tan_angle, density, cos_angle, sin_angle;

         double dim1 = 2*pow((0.75 * ((M_PI / 2.0) - sin((M_PI / 2.0)))), (1.0 / 3.0));
         double dim2 = 2*pow((0.75 * ((M_PI / 2.0) - sin((M_PI / 2.0)))), (1.0 / 3.0));
         double dim3 = 2*pow((0.75 * ((M_PI / 6.0) - sin((M_PI / 6.0)))), (1.0 / 3.0));

         x0001.resize(size * 1);
         y0001.resize(size * 1);
         x1120.resize(size * 3);
         y1120.resize(size * 3);
         x1010.resize(size * 3);
         y1010.resize(size * 3);

         for (int i = 0; i < size; i++)
         {
           random = rg.Random();
           choose = 0;

           totaldensity = 0;
           for (size_t j = 0; j < odfsize; j++)
           {
             density = odf[j];
             totaldensity = totaldensity + density;
             if (random < totaldensity && random >= (totaldensity - density)) choose = static_cast<int> (j);
           }
           h1 = choose % 36;
           h2 = (choose / 36) % 36;
           h3 = choose / (36 * 36);
           random = rg.Random();
           h1 = ((dim1 / 36.0) * h1) + ((dim1 / 36.0) * random) - (dim1 / 2.0);
           random = rg.Random();
           h2 = ((dim2 / 36.0) * h2) + ((dim2 / 36.0) * random) - (dim2 / 2.0);
           random = rg.Random();
           h3 = ((dim3 / 12.0) * h3) + ((dim3 / 12.0) * random) - (dim3 / 2.0);
           hmag = pow((h1 * h1 + h2 * h2 + h3 * h3), 0.5);
           angle = pow((8 * hmag * hmag * hmag), (1.0 / 3.0));
           tan_angle = tan(angle / 2.0);
           n1 = h1 / hmag;
           n2 = h2 / hmag;
           n3 = h3 / hmag;
           r1 = tan_angle * n1;
           r2 = tan_angle * n2;
           r3 = tan_angle * n3;
           cos_angle = cos(angle);
           sin_angle = sin(angle);
           g[0][0] = cos_angle + n1 * n1 * (1 - cos_angle);
           g[0][1] = n1 * n2 * (1 - cos_angle) - n3 * sin_angle;
           g[0][2] = n1 * n3 * (1 - cos_angle) + n2 * sin_angle;
           g[1][0] = n1 * n2 * (1 - cos_angle) + n3 * sin_angle;
           g[1][1] = cos_angle + n2 * n2 * (1 - cos_angle);
           g[1][2] = n2 * n3 * (1 - cos_angle) - n1 * sin_angle;
           g[2][0] = n1 * n3 * (1 - cos_angle) - n2 * sin_angle;
           g[2][1] = n2 * n3 * (1 - cos_angle) + n1 * sin_angle;
           g[2][2] = cos_angle + n3 * n3 * (1 - cos_angle);
           x = g[0][2];
           y = g[1][2];
           z = g[2][2];
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           random = rg.Random();
           x0001[1 * i] = xpf;
           y0001[1 * i] = ypf;
           x = g[0][0];
           y = g[1][0];
           z = g[2][0];
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x1120[3 * i] = xpf;
           y1120[3 * i] = ypf;
           x = (0.5 * g[0][0]) + (0.866025 * g[0][1]);
           y = (0.5 * g[1][0]) + (0.866025 * g[1][1]);
           z = (0.5 * g[2][0]) + (0.866025 * g[2][1]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x1120[3 * i + 1] = xpf;
           y1120[3 * i + 1] = ypf;
           x = (-0.5 * g[0][0]) + (0.866025 * g[0][1]);
           y = (-0.5 * g[1][0]) + (0.866025 * g[1][1]);
           z = (-0.5 * g[2][0]) + (0.866025 * g[2][1]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x1120[3 * i + 2] = xpf;
           y1120[3 * i + 2] = ypf;
           x = (0.866025 * g[0][0]) + (0.5 * g[0][1]);
           y = (0.866025 * g[1][0]) + (0.5 * g[1][1]);
           z = (0.866025 * g[2][0]) + (0.5 * g[2][1]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x1010[3 * i] = xpf;
           y1010[3 * i] = ypf;
           x = g[0][1];
           y = g[1][1];
           z = g[2][1];
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x1010[3 * i + 1] = xpf;
           y1010[3 * i + 1] = ypf;
           x = (-0.866025 * g[0][0]) + (0.5 * g[0][1]);
           y = (-0.866025 * g[1][0]) + (0.5 * g[1][1]);
           z = (-0.866025 * g[2][0]) + (0.5 * g[2][1]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x1010[3 * i + 2] = xpf;
           y1010[3 * i + 2] = ypf;
         }
         return err;
       }

       /**
        * @brief  This method will generate ODF data for a OrthoRhombic material and
        * generate 3 scatter plots which are the
        * <001>, <011> and <111> directions. The method is a C++ Template which the parameter
        * type is a std::vector conforming class type that holds the data.
        * QVector falls into this category. The input data for the
        * euler angles is in Columnar fashion instead of row major format.
        * @param e1s The first euler angles (input)
        * @param e2s The second euler angles (input)
        * @param e3s The third euler angles (input)
        * @param weights Array of weights values. (input)
        * @param sigmas Array of sigma values. (input)
        * @param x001 X Values of the [001] axis PF Scatter plot (Output)
        * @param y001 Y Values of the [001] axis PF Scatter plot (Output)
        * @param x011 X Values of the [011] axis PF Scatter plot (Output)
        * @param y011 Y Values of the [011] axis PF Scatter plot (Output)
        * @param x111 X Values of the [111] axis PF Scatter plot (Output)
        * @param y111 Y Values of the [111] axis PF Scatter plot (Output)
        * @param size The number of points for the Scatter Plot
        */
       template<typename T>
       int GenOrthoRhombicODFPlotData(T e1s, T e2s, T e3s, T weights, T sigmas,
                       T &x001, T &y001, T &x011, T &y011, T &x111, T &y111,
                       int size)
       {
         static const size_t odfsize = 46656;
         double totalweight = 0;
         T odf;
         odf.resize(odfsize);
         Texture::calculateOrthoRhombicODFData(e1s, e2s, e3s, weights, sigmas, true, odf, totalweight);

         AIMRandomNG rg;
         /* Get a seed value based off the system clock. The issue is that this will
          * be a 64 bit unsigned integer where the high 32 bits will basically not
          * change where as the lower 32 bits will. The following lines of code will
          * pull off the low 32 bits from the number. This operation depends on most
          * significant byte ordering which is different between Big Endian and
          * Little Endian machines. For Big endian machines the Most Significant Byte
          * (MSB) is the first 32 bits. For Little Endian machines the MSB is the
          * second 32 bits.
          */
         unsigned long long int seed = MXA::getMilliSeconds();
         unsigned int* seedPtr = reinterpret_cast<unsigned int*>(&seed);
   #if CMP_WORDS_BIGENDIAN
         rg.RandomInit(seedPtr[1]);
   #else
         rg.RandomInit(seedPtr[0]);
   #endif
         int err = 0;
         int choose;
         double g[3][3];
         double x, y, z;
         double xpf, ypf;
         double xpfa, ypfa;
         double totaldensity;
         double hmag;
         double angle;
         double r1, r2, r3;
         double h1, h2, h3;
         double n1, n2, n3;
         double random, tan_angle, density, cos_angle, sin_angle;

         double dim1 = 2*pow((0.75 * ((M_PI / 2.0) - sin((M_PI / 2.0)))), (1.0 / 3.0));
         double dim2 = 2*pow((0.75 * ((M_PI / 2.0) - sin((M_PI / 2.0)))), (1.0 / 3.0));
         double dim3 = 2*pow((0.75 * ((M_PI / 2.0) - sin((M_PI / 2.0)))), (1.0 / 3.0));

         x001.resize(size * 3);
         y001.resize(size * 3);
         x011.resize(size * 6);
         y011.resize(size * 6);
         x111.resize(size * 4);
         y111.resize(size * 4);

         for (int i = 0; i < size; i++)
         {
           random = rg.Random();
           choose = 0;

           totaldensity = 0;
           for (size_t j = 0; j < odfsize; j++)
           {
             density = odf[j];
             totaldensity = totaldensity + density;
             if (random < totaldensity && random >= (totaldensity - density)) choose = static_cast<int> (j);
           }
           h1 = choose % 36;
           h2 = (choose / 36) % 36;
           h3 = choose / (36 * 36);
           random = rg.Random();
           h1 = ((dim1 / 36.0) * h1) + ((dim1 / 36.0) * random) - (dim1 / 2.0);
           random = rg.Random();
           h2 = ((dim2 / 36.0) * h2) + ((dim2 / 36.0) * random) - (dim2 / 2.0);
           random = rg.Random();
           h3 = ((dim3 / 36.0) * h3) + ((dim3 / 36.0) * random) - (dim3 / 2.0);
           hmag = pow((h1 * h1 + h2 * h2 + h3 * h3), 0.5);
           angle = pow((8 * hmag * hmag * hmag), (1.0 / 3.0));
           tan_angle = tan(angle / 2.0);
           n1 = h1 / hmag;
           n2 = h2 / hmag;
           n3 = h3 / hmag;
           r1 = tan_angle * n1;
           r2 = tan_angle * n2;
           r3 = tan_angle * n3;
           cos_angle = cos(angle);
           sin_angle = sin(angle);
           g[0][0] = cos_angle + n1 * n1 * (1 - cos_angle);
           g[0][1] = n1 * n2 * (1 - cos_angle) - n3 * sin_angle;
           g[0][2] = n1 * n3 * (1 - cos_angle) + n2 * sin_angle;
           g[1][0] = n1 * n2 * (1 - cos_angle) + n3 * sin_angle;
           g[1][1] = cos_angle + n2 * n2 * (1 - cos_angle);
           g[1][2] = n2 * n3 * (1 - cos_angle) - n1 * sin_angle;
           g[2][0] = n1 * n3 * (1 - cos_angle) - n2 * sin_angle;
           g[2][1] = n2 * n3 * (1 - cos_angle) + n1 * sin_angle;
           g[2][2] = cos_angle + n3 * n3 * (1 - cos_angle);
           x = g[0][0];
           y = g[1][0];
           z = g[2][0];
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           random = rg.Random();
           x001[3 * i] = xpf;
           y001[3 * i] = ypf;
           x = g[0][1];
           y = g[1][1];
           z = g[2][1];
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x001[3 * i + 1] = xpf;
           y001[3 * i + 1] = ypf;
           x = g[0][2];
           y = g[1][2];
           z = g[2][2];
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x001[3 * i + 2] = xpf;
           y001[3 * i + 2] = ypf;
           x = 0.707107 * (g[0][0] + g[0][1]);
           y = 0.707107 * (g[1][0] + g[1][1]);
           z = 0.707107 * (g[2][0] + g[2][1]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x011[6 * i] = xpf;
           y011[6 * i] = ypf;
           x = 0.707107 * (g[0][1] + g[0][2]);
           y = 0.707107 * (g[1][1] + g[1][2]);
           z = 0.707107 * (g[2][1] + g[2][2]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x011[6 * i + 1] = xpf;
           y011[6 * i + 1] = ypf;
           x = 0.707107 * (g[0][2] + g[0][0]);
           y = 0.707107 * (g[1][2] + g[1][0]);
           z = 0.707107 * (g[2][2] + g[2][0]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x011[6 * i + 2] = xpf;
           y011[6 * i + 2] = ypf;
           x = 0.707107 * (g[0][0] - g[0][1]);
           y = 0.707107 * (g[1][0] - g[1][1]);
           z = 0.707107 * (g[2][0] - g[2][1]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x011[6 * i + 3] = xpf;
           y011[6 * i + 3] = ypf;
           x = 0.707107 * (g[0][1] - g[0][2]);
           y = 0.707107 * (g[1][1] - g[1][2]);
           z = 0.707107 * (g[2][1] - g[2][2]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x011[6 * i + 4] = xpf;
           y011[6 * i + 4] = ypf;
           x = 0.707107 * (g[0][2] - g[0][0]);
           y = 0.707107 * (g[1][2] - g[1][0]);
           z = 0.707107 * (g[2][2] - g[2][0]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x011[6 * i + 5] = xpf;
           y011[6 * i + 5] = ypf;
           x = 0.57735 * (g[0][0] + g[0][1] + g[0][2]);
           y = 0.57735 * (g[1][0] + g[1][1] + g[1][2]);
           z = 0.57735 * (g[2][0] + g[2][1] + g[2][2]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x111[4 * i] = xpf;
           y111[4 * i] = ypf;
           x = 0.57735 * (g[0][0] + g[0][1] - g[0][2]);
           y = 0.57735 * (g[1][0] + g[1][1] - g[1][2]);
           z = 0.57735 * (g[2][0] + g[2][1] - g[2][2]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x111[4 * i + 1] = xpf;
           y111[4 * i + 1] = ypf;
           x = 0.57735 * (g[0][0] - g[0][1] + g[0][2]);
           y = 0.57735 * (g[1][0] - g[1][1] + g[1][2]);
           z = 0.57735 * (g[2][0] - g[2][1] + g[2][2]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x111[4 * i + 2] = xpf;
           y111[4 * i + 2] = ypf;
           x = 0.57735 * (-g[0][0] + g[0][1] + g[0][2]);
           y = 0.57735 * (-g[1][0] + g[1][1] + g[1][2]);
           z = 0.57735 * (-g[2][0] + g[2][1] + g[2][2]);
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           x111[4 * i + 3] = xpf;
           y111[4 * i + 3] = ypf;
         }
         return err;
       }






       /**
        * @brief  This method will generate ODF data for a OrthoRhombic material and
        * generate 3 scatter plots which are the
        * <001>, <011> and <111> directions. The method is a C++ Template which the parameter
        * type is a std::vector conforming class type that holds the data.
        * QVector falls into this category. The input data for the
        * euler angles is in Columnar fashion instead of row major format.
        * @param e1s The first euler angles (input)
        * @param e2s The second euler angles (input)
        * @param e3s The third euler angles (input)
        * @param weights Array of weights values. (input)
        * @param sigmas Array of sigma values. (input)
        * @param xA X Values of the A axis PF Scatter plot (Output)
        * @param yA Y Values of the A axis PF Scatter plot (Output)
        * @param xB X Values of the B axis PF Scatter plot (Output)
        * @param yB Y Values of the B axis PF Scatter plot (Output)
        * @param xC X Values of the C axis PF Scatter plot (Output)
        * @param yC Y Values of the C axis PF Scatter plot (Output)
        * @param size The number of points for the Scatter Plot
        */
       template<typename T>
       int GenAxisODFPlotData(T e1s, T e2s, T e3s, T weights, T sigmas,
                       T &xA, T &yA, T &xB, T &yB, T &xC, T &yC,
                       int size)
       {
         static const size_t odfsize = 46656;
         double totalweight = 0;
         T odf;
         odf.resize(odfsize);
         Texture::calculateOrthoRhombicODFData(e1s, e2s, e3s, weights, sigmas, true, odf, totalweight);

         AIMRandomNG rg;
         /* Get a seed value based off the system clock. The issue is that this will
          * be a 64 bit unsigned integer where the high 32 bits will basically not
          * change where as the lower 32 bits will. The following lines of code will
          * pull off the low 32 bits from the number. This operation depends on most
          * significant byte ordering which is different between Big Endian and
          * Little Endian machines. For Big endian machines the Most Significant Byte
          * (MSB) is the first 32 bits. For Little Endian machines the MSB is the
          * second 32 bits.
          */
         unsigned long long int seed = MXA::getMilliSeconds();
         unsigned int* seedPtr = reinterpret_cast<unsigned int*>(&seed);
   #if CMP_WORDS_BIGENDIAN
         rg.RandomInit(seedPtr[1]);
   #else
         rg.RandomInit(seedPtr[0]);
   #endif
         int err = 0;
         int choose;
         double g[3][3];
         double x, y, z;
         double xpf, ypf;
         double xpfa, ypfa;
         double totaldensity;
         double hmag;
         double angle;
         double r1, r2, r3;
         double h1, h2, h3;
         double n1, n2, n3;
         double random, tan_angle, density, cos_angle, sin_angle;

         double dim1 = 2*pow((0.75 * ((M_PI / 2.0) - sin((M_PI / 2.0)))), (1.0 / 3.0));
         double dim2 = 2*pow((0.75 * ((M_PI / 2.0) - sin((M_PI / 2.0)))), (1.0 / 3.0));
         double dim3 = 2*pow((0.75 * ((M_PI / 2.0) - sin((M_PI / 2.0)))), (1.0 / 3.0));

         xA.resize(size * 1);
         yA.resize(size * 1);
         xB.resize(size * 1);
         yB.resize(size * 1);
         xC.resize(size * 1);
         yC.resize(size * 1);

         for (int i = 0; i < size; i++)
         {
           random = rg.Random();
           choose = 0;

           totaldensity = 0;
           for (size_t j = 0; j < odfsize; j++)
           {
             density = odf[j];
             totaldensity = totaldensity + density;
             if (random < totaldensity && random >= (totaldensity - density)) choose = static_cast<int> (j);
           }
           h1 = choose % 36;
           h2 = (choose / 36) % 36;
           h3 = choose / (36 * 36);
           random = rg.Random();
           h1 = ((dim1 / 36.0) * h1) + ((dim1 / 36.0) * random) - (dim1 / 2.0);
           random = rg.Random();
           h2 = ((dim2 / 36.0) * h2) + ((dim2 / 36.0) * random) - (dim2 / 2.0);
           random = rg.Random();
           h3 = ((dim3 / 36.0) * h3) + ((dim3 / 36.0) * random) - (dim3 / 2.0);
           hmag = pow((h1 * h1 + h2 * h2 + h3 * h3), 0.5);
           angle = pow((8 * hmag * hmag * hmag), (1.0 / 3.0));
           tan_angle = tan(angle / 2.0);
           n1 = h1 / hmag;
           n2 = h2 / hmag;
           n3 = h3 / hmag;
           r1 = tan_angle * n1;
           r2 = tan_angle * n2;
           r3 = tan_angle * n3;
           cos_angle = cos(angle);
           sin_angle = sin(angle);
           g[0][0] = cos_angle + n1 * n1 * (1 - cos_angle);
           g[0][1] = n1 * n2 * (1 - cos_angle) - n3 * sin_angle;
           g[0][2] = n1 * n3 * (1 - cos_angle) + n2 * sin_angle;
           g[1][0] = n1 * n2 * (1 - cos_angle) + n3 * sin_angle;
           g[1][1] = cos_angle + n2 * n2 * (1 - cos_angle);
           g[1][2] = n2 * n3 * (1 - cos_angle) - n1 * sin_angle;
           g[2][0] = n1 * n3 * (1 - cos_angle) - n2 * sin_angle;
           g[2][1] = n2 * n3 * (1 - cos_angle) + n1 * sin_angle;
           g[2][2] = cos_angle + n3 * n3 * (1 - cos_angle);
           x = g[0][0];
           y = g[1][0];
           z = g[2][0];
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           random = rg.Random();
           xA[1 * i] = xpf;
           yA[1 * i] = ypf;
           x = g[0][1];
           y = g[1][1];
           z = g[2][1];
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           xB[1 * i] = xpf;
           yB[1 * i] = ypf;
           x = g[0][2];
           y = g[1][2];
           z = g[2][2];
           if (z < 0) x = -x, y = -y, z = -z;
           xpf = y - (y * (z / (z + 1)));
           ypf = x - (x * (z / (z + 1)));
           xC[1 * i] = xpf;
           yC[1 * i] = ypf;
         }
         return err;
       }





  private:
    StatsGen(const StatsGen&); // Copy Constructor Not Implemented
    void operator=(const StatsGen&); // Operator '=' Not Implemented
};

#endif /* _STATSGEN_H_ */
