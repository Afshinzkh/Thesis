#include "../Headers/HullWhite.h"
#include "../Headers/hwDE.h"

namespace Calibration
{
  #define boundaryMIN 0.000001;

  void hwDE::runDE()
  {

// TODO:first I need to run a whole DE to find the best short rate for GDP
// this would be a simple vasicek nextRate calculation and check if it
// is close to the value I have for my GDP
// the value for gdp should be read from what I will take from internet
// monthly gdp of a country (sth between 0.5 to 4 %)

 /****************************************************************************/
 /******************** STEP 1 : Initialize *********************************/
 /****************************************************************************/
  	// Select the DE Parameters as follows, NP  : Population Size >= 4
    //                                      F   : Scale Factor
  	//                                      CR  : Crossover Ratio [0,1]

    const int NP = 15;
    double F = 0.85;
    double CR = 0.7;

    // Creat a population matrix P with the size of [NP * mpCount]
    // where mpCount is the count of Model Parameters;
    // for vasicek it is 3 for alpha, beta and sigma
    // for risklab it is 9 : alphaS, betaS, sigmaS, alphaY, betaY, sigmaY,
    // sigmaZ, bY, bZ
    int mpCount = 4;
    // std::array< std::array <double, mpCount> , NP > P;
    std::vector < std::vector <double> > P(NP,std::vector<double> (mpCount,0));
    // the parameters sequence in P would be as follows
    // (alphaS, betaS, sigmaS, alphaY, betaY, sigmaY, sigmaZ, bY, bZ)


    // Define the Random Generator here
    // to get random values for model Parameters e.g. alpha1, sigma1, alpha2,
    // sigma2 and rho
    // Upper and Lower bounds for them are also defined here
    std::array<double, 4> upperBound = {12.0, 1.0, 1.0};
    std::array<double, 4> lowerBound = {0.0, 0.00001, -1.0};

    // Pick Random Variables for model parameters
    // P[i][0] = a random double value for alpha1
    // P[i][1] = a random double value for sigma1
    // P[i][2] = a random double value for alpha2
    // P[i][3] = a random double value for sigma2
    // P[i][4] = a random double value for rho

    // Define the Random Device
    std::random_device rd;
	 	std::mt19937 gen(rd());
	 	std::uniform_real_distribution<long double> alphaRands(lowerBound[0],upperBound[0]);
    std::uniform_real_distribution<long double> sigmaRands(lowerBound[1],upperBound[1]);
    std::uniform_real_distribution<long double> rhoRands(lowerBound[2],upperBound[2]);

    // start the calculation time here


    for(int i = 0; i < NP; i++)
    {
      // alpha1, sigma1, alpha2, sigma2, rho
      P[i][0] = alphaRands(gen);  // alpha1
      P[i][1] = sigmaRands(gen);  // sigma1
      P[i][2] = alphaRands(gen);  // alpha2
      P[i][3] = sigmaRands(gen);  // sigma2
      P[i][3] = rhoRands(gen);    // rho
    }


    // Then I do the main second DE
    // Define Tolerance for Error
    double tol = 0.00001;
    avgError = 1.0;
    int maxIter = 50;
    int iter = 0;
    loopCount = 0;




    std::array<double,9> tau = {0.25, 1, 3, 5, 7, 10, 15, 20, 30};
  	// Initialize r0 to a given value;
  	double r0 = 0.0006;
    // auto v = new Vasicek(r0, tau);
    auto hw = new HullWhite(r0, tau);

/****************************************************************************/
/******************** STEP 2 : DE LOOP **************************************/
/****************************************************************************/
    double pError [NP];
    while (tol < avgError && iter < maxIter)
    {
        // Calculate the Vasicek/risklab Error for each of these populations
        // input alpha, beta, sigma; output : error;


        double sum = 0.0;
        for(int i = 0; i < NP; i++)

        {

            hw->setParameters(P[i][0], P[i][1], P[i][2], P[i][3], P[i][4]);
            hw->setMrktArray(crrntMonthMrktData);
            hw->run();
            pError[i] = hw->getError();
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

  /****************************************************************************/
  /******************** STEP 3 : Mutation *************************************/
  /****************************************************************************/
        // Do the Mutation Stage as follows
        // define a mutated population as mutP
        // std::array< std::array <long double, mpCount> , NP > mutP;
        std::vector < std::vector <double> > mutP(NP,std::vector<double> (mpCount,0));
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

            mutP[i][3] = P[id1][3] + F * (P[id2][3] - P[id3][3]);
            if(mutP[i][3] > upperBound[0])
                mutP[i][3] = upperBound[0] - boundaryMIN;
            if(mutP[i][3] < lowerBound[0])
                mutP[i][3] = lowerBound[0] + boundaryMIN;

            mutP[i][4] = P[id1][4] + F * (P[id2][4] - P[id3][4]);
            if(mutP[i][4] > upperBound[1])
                mutP[i][4] = upperBound[1] - boundaryMIN;
            if(mutP[i][4] < lowerBound[1])
                mutP[i][4] = lowerBound[1] + boundaryMIN;

        }

  /****************************************************************************/
  /******************** STEP 4 : Crossover ************************************/
  /****************************************************************************/
        // Crossover the mutated with the origital Poupation
        // define a Crossover Population as crP
        // std::array< std::array <long double, mpCount> , NP > crP;
        std::vector < std::vector <double> > crP(NP,std::vector<double> (mpCount,0));
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
            hw->setParameters(crP[i][0], crP[i][1], crP[i][2], crP[i][3], crP[i][4]);
            hw->setMrktArray(crrntMonthMrktData);
            hw->run();
            crError[i] = hw->getError();
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
               P[i][3] = crP[i][3];
               P[i][4] = crP[i][4];
            }
        }
        // So with this new P you can again check the error and go ahead and repeat

        iter++;

    }// end of while loop


    // Print out the final Values and also add them to the main arrays
    double finAlpha1 = 0.0;
    double finSigma1 = 0.0;
    double finAlpha2 = 0.0;
    double finSigma2 = 0.0;
    double finRho = 0.0;
    for(int i = 0; i < NP; i++)
    {
      finAlpha1 += P[i][0];
      finSigma1 += P[i][1];
      finAlpha2 += P[i][2];
      finSigma2 += P[i][3];
      finRho += P[i][4];
    }
    // Copy them to private values
    alpha1 = finAlpha1/NP;
    sigma1 = finSigma1/NP;
    alpha2 = finAlpha2/NP;
    sigma2 = finSigma2/NP;
    rho = finRho/NP;
  /****************************************************************************/
  /***************** STEP 4 : Final Calculation *******************************/
  /****************************************************************************/
    //get the final values with final parameters
    double u1 = hw->nextURate();
    double r1 = hw->nextRRate(u1);
    for (size_t i = 0; i < 9; i++) {
      crrntMonthMdlData[i] = hw->getYield(r1, u1, tau[i]);
    }


  /****************************************************************************/
	/*************************** STEP 4 : Print Out *****************************/
	/****************************************************************************/

      std::cout << "\nfinal alpha1:" <<  alpha1 <<std::endl;
      std::cout << "final sigma1:" << sigma1 <<std::endl;
      std::cout << "\nfinal alpha2:" <<  alpha2 <<std::endl;
      std::cout << "final sigma2:" << sigma2 <<std::endl;
      std::cout << "\nfinal rho:" <<  rho <<std::endl;

      std::cout << "Average Error for loop :" << loopCount;
      std::cout << "\t is : " << avgError << std::endl;

      // for (size_t j = 0; j < 9; j++) {
      //   std::cout << "y for maturity: "  << tau[j] << "\t is: \t" << crrntMonthMdlData[j] << std::endl;
      // }

  }// DE::runDE



/****************************************************************************/
/******************** Setters and Getters are here **************************/
/****************************************************************************/

  hwDE::hwDE()
  {
    // methodName = mName;
  }

  const double& hwDE::getAlpha1() const { return alpha1; }
  const double& hwDE::getSigma1() const { return sigma1; }
  const double& hwDE::getAlpha2() const { return alpha2; }
  const double& hwDE::getSigma2() const { return sigma2; }
  const double& hwDE::getRho() const { return rho; }


  const double& hwDE::getError() const { return avgError; }
  const int& hwDE::getIter() const { return loopCount; }
  const double& hwDE::getTime() const { return calTime; }

  const std::array<double, 9>& hwDE::getMdlArray() const
  {
    return crrntMonthMdlData;
  }

  void hwDE::setMrktArray(std::array<double, 9> const& mrktData)
  {
    crrntMonthMrktData = mrktData;
  }

}// namespace Calibration
