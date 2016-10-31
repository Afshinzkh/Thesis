#include "../Headers/Vasicek.h"
#include "../Headers/DE.h"

namespace Calibration
{
  #define boundaryMIN 0.000001;

  template < size_t N >
  void DE::runDE(std::string methodName,
            std::array<double, N> const &crrntMonthMrktData)
  {
  	// Select the DE Parameters as follows, NP  : Population Size >= 4
    //                                      F   : Scale Factor
  	//                                      CR  : Crossover Ratio [0,1]

      const int NP = 15;
      double F = 0.85;
      double CR = 0.7;

      // Creat a population matrix P with the size of [NP * mpCount]
      // where mpCount is the count of Model Parameters;
      // e.g. for vasicek it is 3 for alpha, beta and sigma
      const int mpCount = 3;
      std::array< std::array <long double, mpCount> , NP > P;

      // Define the Random Generator here
      // to get random values for model Parameters e.g. alpha, beta, sigma
      // Upper and Lower bounds for them are also defined here
      std::array<double, 3> upperBound = {12.0, 0.005, 1};
      std::array<double, 3> lowerBound = {0.0, 0.0001, 0.0};

      // Pick Random Variables for model parameters
      // P[i][0] = a random double value for alpha with restrictions 0<alpha<12
      // P[i][1] = a random double value for beta with restrictions beta in R
      // P[i][2] = a random double value for sigma with restrictions 0<sigma

      // Define the Random Device
      std::random_device rd;
  	 	std::mt19937 gen(rd());
  	 	std::uniform_real_distribution<long double> alphaRands(lowerBound[0],upperBound[0]);
      std::uniform_real_distribution<long double> betaRands(lowerBound[1],upperBound[1]);
      std::uniform_real_distribution<long double> sigmaRands(lowerBound[2],upperBound[2]);

      for(int i = 0; i < NP; i++)
      {
        P[i][0] = alphaRands(gen);
        P[i][1] = betaRands(gen);
        P[i][2] = sigmaRands(gen);
      }


      // Define Tolerance for Error
      double tol = 0.00001;
      double avgError = 1.0;
      int maxIter = 60;
      int iter = 0;
      int loopCount = 0;

      // std::shared_ptr<Base> v;
      //
      // if(arg == "Vasicek") {
      //   v.reset(new Vasicek)
      // } else {
      //   v.reset(new risklab)
      // }
      auto v = new Vasicek;

      // The loop of DE Starts Here
      while (tol < avgError && iter < maxIter)
      {
          // Calculate the Vasicek/risklab Error for each of these populations
          // input alpha, beta, sigma; output : error;

          double pError [NP];
          double sum = 0.0;
          for(int i = 0; i < NP; i++)
          {
              pError[i] = v->run(P[i][0], P[i][1], P[i][2], crrntMonthMrktData);
              sum += pError[i];
          }

          // compute the average Error
          avgError = sum/NP;
          loopCount++;

          //   std::cout << " * " << std::flush;
          // if (loopCount % 10 == 0 )
          //    std::cout << std::endl;

             std::cout << "Average Error for Calculation loop :" << loopCount;
             std::cout << "\t is : " << avgError << std::endl;
          // Do the Mutation Stage as follows
          // define a mutated population as mutP
          std::array< std::array <long double, mpCount> , NP > mutP;

          // Define the Random Generator here
          // to get random values for model Parameters e.g. alpha, beta, sigma
      	 	std::uniform_int_distribution<> indexRands(0,NP-1);
          for (int i = 0; i < NP; i++)
          {
              // create 3 random indexes for each i the new mutated population set
              // with restriction  0<=id1,id2,id3<NP
              // and all the indexes should be different from each other and also from i
              int id1 = indexRands(gen);
              int id2 = indexRands(gen);
              int id3 = indexRands(gen);
              while (id1 == i)  id1 = indexRands(gen);
              while (id2 == i || id2 == id1) id2 = indexRands(gen);
              while (id3 == i || id3 == id1 || id3 == id2) id3 = indexRands(gen);

              // Check the boundaries
              mutP[i][0] = P[id1][0] + F * (P[id2][0] - P[id3][0]);
              if(mutP[i][0] > upperBound[0])
                  mutP[i][0] = upperBound[0] - boundaryMIN;
              if(mutP[i][0] < lowerBound[0])
                  mutP[i][0] = lowerBound[0] + boundaryMIN;

              mutP[i][1] = P[id1][1] + F * (P[id2][1] - P[id3][1]);
              if(mutP[i][1] > upperBound[1])
                  mutP[i][1] = upperBound[1] - boundaryMIN;
              if(mutP[i][1] < lowerBound[1])
                  mutP[i][1] = lowerBound[1] + boundaryMIN;

              mutP[i][2] = P[id1][2] + F * (P[id2][2] - P[id3][2]);
              if(mutP[i][2] > upperBound[2])
                  mutP[i][2] = upperBound[2] - boundaryMIN;
              if(mutP[i][2] < lowerBound[2])
                  mutP[i][2] = lowerBound[2] + boundaryMIN;

          }

          // Crossover the mutated with the origital Poupation
          // define a Crossover Population as crP
          std::array< std::array <long double, mpCount> , NP > crP;

          // Define the random Distribution U(0,1)
          std::uniform_real_distribution<> crRands(0,1);
          // Define random intex for model parameter count
          std::uniform_int_distribution<> mpRand(0,mpCount-1);
          // choose each model Parameter  as follows
          for (int i = 0; i < NP; i++)
          {
              for (int j = 0; j < mpCount; j++)
              {
                  if(crRands(gen) < CR || mpRand(gen) == j)
                       crP[i][j] = mutP[i][j];
                  else
                       crP[i][j] = P[i][j];
              }
          }

          // Now that we have the Crossover Population we can calculate it's Errors
          double crError [NP];
          for(int i = 0; i < NP; i++)
          {
              crError[i] = v->run(crP[i][0], crP[i][1], crP[i][2], crrntMonthMrktData);
          }

          // Now you can compare the Error and if the error of one crossover population
          // is less than the error of one original population you copy that
          for(int i = 0; i < NP; i++)
          {
               if (crError[i] < pError [i])
              {
                 P[i][0] = crP[i][0];
                 P[i][1] = crP[i][1];
                 P[i][2] = crP[i][2];
              }
          }
          // So with this new P you can again check the error and go ahead and repeat

          iter++;

      }

      // Print out the final Values and also add them to the main arrays
      double finAlpha, finBeta, finSigma;
      for(int i = 0; i < NP; i++)
      {
        finAlpha += P[i][0];
        finBeta += P[i][1];
        finSigma += P[i][2];
      }
      finAlpha = finAlpha/NP;
      finBeta = finBeta/NP;
      finSigma = finSigma/NP;

      std::cout << "\nfinal alpha:" <<  finAlpha <<std::endl;
      std::cout << "final beta:" << finBeta <<std::endl;
      std::cout << "final sigma:" << finSigma <<std::endl;

      std::cout << "Average Error for Calculation loop :" << loopCount;
      std::cout << "\t is : " << avgError << std::endl;

      // JUST A TEST TO GET THE FINAL YIELDS
      double r1 = v->nextRate(0.0006, finAlpha, finBeta, finSigma);
      std::array<double,9> tau = {0.25, 1, 3, 5, 7, 10, 15, 20, 30};
      std::array<double,9> y;
      for (size_t i = 0; i < 9; i++) {
        y[i] = v->getYield(r1, finAlpha, finBeta, finSigma, tau[i]);
        std::cout << "y for maturity: "  << tau[i] << "\t is: \t" << y[i] << std::endl;
      }
      // TODO: get return values from deMain as a double array
  }
}
