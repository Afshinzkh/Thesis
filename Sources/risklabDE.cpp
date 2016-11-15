#include "../Headers/risklab.h"
#include "../Headers/risklabDE.h"

namespace Calibration
{
  #define boundaryMIN 0.000001;

  void risklabDE::runDE()
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
    int mpCount = 9;
    // std::array< std::array <double, mpCount> , NP > P;
    std::vector < std::vector <double> > P(NP,std::vector<double> (mpCount,0));
    // the parameters sequence in P would be as follows
    // (alphaS, betaS, sigmaS, alphaY, betaY, sigmaY, sigmaZ, bY, bZ)

    // man meti ghazal to razi
    // payam elahe pedram
    // sahand moin
    // raoofi ashkan
    // nokhod loobia goosht ostekhoon sib zamani goje limo amani
    // rob e goje noon lavash sabzi khordan torobche
    // piaz torshi litte
    // ghablame bozorg

    // TODO: right now NP and mpcount cannot be private variables because they
    // are defining the array and the array cannot get non const
    // this will be fixed later that we do vectorization and shit

    // Define the Random Generator here
    // to get random values for model Parameters e.g. alpha, beta, sigma, bY/bZ
    // bY an bZ are weight for each Y and Z
    // Upper and Lower bounds for them are also defined here
    std::array<double, 4> upperBound = {12.0, 0.05, 1.0, 1.0};
    std::array<double, 4> lowerBound = {0.0, 0.00001, 0.00001, 0.0};

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
    std::uniform_real_distribution<long double> weightRands(lowerBound[3],upperBound[3]);

    // start the calculation time here
    auto start = std::chrono::steady_clock::now();

    for(int i = 0; i < NP; i++)
    {
      // alphaS, betaS, sigmaS, alphaY, betaY, sigmaY, sigmaZ, bY, bZ
      P[i][0] = alphaRands(gen);  // alphaS
      P[i][1] = betaRands(gen);   // betaS
      P[i][2] = sigmaRands(gen);  // sigmaS
      P[i][3] = alphaRands(gen);  // alphaY
      P[i][4] = betaRands(gen);   // betaY
      P[i][5] = sigmaRands(gen);  // sigmaY
      P[i][6] = sigmaRands(gen);  // sigmaZ
      P[i][7] = weightRands(gen); // bY
      P[i][8] = weightRands(gen); // bZ
    }


    // Then I do the main second DE
    // Define Tolerance for Error
    double tol = 0.00001;
    avgError = 1.0;
    int maxIter = 50;
    int iter = 0;
    loopCount = 0;



    //TODO: add the risklab class and choose them like this
    // std::shared_ptr<Base> v;
    //
    // if(arg == "Vasicek") {
    //   v.reset(new Vasicek)
    // } else {
    //   v.reset(new risklab)
    // }

    std::array<double,9> tau = {0.25, 1, 3, 5, 7, 10, 15, 20, 30};
  	// Initialize r0 to a given value;
  	double r0 = 0.0006;
    double y0 = 0.0012;
    double z0 = 0.0006;
    // auto v = new Vasicek(r0, tau);
    auto r = new risklab(r0, y0, z0, tau);

    // set GDP parameters

    r->setGDPParameters( alphaZ, betaZ );
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

            r->setParameters(P[i][0], P[i][1], P[i][2], P[i][3], P[i][4],
                    P[i][5], P[i][6], P[i][7], P[i][8]);
            r->setMrktArray(crrntMonthMrktData);
            r->run();
            pError[i] = r->getError();
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

            mutP[i][5] = P[id1][5] + F * (P[id2][5] - P[id3][5]);
            if(mutP[i][5] > upperBound[2])
                mutP[i][5] = upperBound[2] - boundaryMIN;
            if(mutP[i][5] < lowerBound[2])
                mutP[i][5] = lowerBound[2] + boundaryMIN;

            mutP[i][6] = P[id1][6] + F * (P[id2][6] - P[id3][6]);
            if(mutP[i][6] > upperBound[0])
                mutP[i][6] = upperBound[0] - boundaryMIN;
            if(mutP[i][6] < lowerBound[0])
                mutP[i][6] = lowerBound[0] + boundaryMIN;

            mutP[i][7] = P[id1][7] + F * (P[id2][7] - P[id3][7]);
            if(mutP[i][7] > upperBound[1])
                mutP[i][7] = upperBound[1] - boundaryMIN;
            if(mutP[i][7] < lowerBound[1])
                mutP[i][7] = lowerBound[1] + boundaryMIN;

            mutP[i][8] = P[id1][8] + F * (P[id2][8] - P[id3][8]);
            if(mutP[i][8] > upperBound[2])
                mutP[i][8] = upperBound[2] - boundaryMIN;
            if(mutP[i][8] < lowerBound[2])
                mutP[i][8] = lowerBound[2] + boundaryMIN;

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
            r->setParameters(crP[i][0], crP[i][1], crP[i][2], crP[i][3], crP[i][4],
                    crP[i][5], crP[i][6], crP[i][7], crP[i][8]);

            r->setMrktArray(crrntMonthMrktData);
            r->run();
            crError[i] = r->getError();
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
               P[i][5] = crP[i][5];
               P[i][6] = crP[i][6];
               P[i][7] = crP[i][7];
               P[i][8] = crP[i][8];
            }
        }
        // So with this new P you can again check the error and go ahead and repeat

        iter++;

    }// end of while loop


    // Print out the final Values and also add them to the main arrays
    double finAlphaS = 0.0;
    double finBetaS = 0.0;
    double finSigmaS = 0.0;
    double finAlphaY = 0.0;
    double finBetaY = 0.0;
    double finSigmaY = 0.0;
    double finSigmaZ = 0.0;
    double finWeightY = 0.0;
    double finWeightZ = 0.0;
    for(int i = 0; i < NP; i++)
    {
      finAlphaS += P[i][0];
      finBetaS += P[i][1];
      finSigmaS += P[i][2];
      finAlphaY += P[i][3];
      finBetaY += P[i][4];
      finSigmaY += P[i][5];
      finSigmaZ += P[i][6];
      finWeightY += P[i][7];
      finWeightZ += P[i][8];
    }
    // Copy them to private values
    alphaS = finAlphaS/NP;
    betaS = finBetaS/NP;
    sigmaS = finSigmaS/NP;
    alphaY = finAlphaY/NP;
    betaY = finBetaY/NP;
    sigmaY = finSigmaY/NP;
    sigmaZ = finSigmaZ/NP;
    weightY = finWeightY/NP;
    weightZ = finWeightZ/NP;

  /****************************************************************************/
  /***************** STEP 4 : Final Calculation *******************************/
  /****************************************************************************/
    // //get the final values with final parameters
    // double r1 = r->nextRate();
    // for (size_t i = 0; i < 9; i++) {
    //   crrntMonthMdlData[i] = r->getYield(r1, tau[i]);
    // }

    // end the Calculation time here
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> durationCount = end - start;
    calTime = durationCount.count();
  /****************************************************************************/
	/*************************** STEP 4 : Print Out *****************************/
	/****************************************************************************/

      std::cout << "\nfinal alphaS:" <<  alphaS <<std::endl;
      std::cout << "final betaS:" << betaS <<std::endl;
      std::cout << "final sigmaS:" << sigmaS <<std::endl;
      std::cout << "\nfinal alphaY:" <<  alphaY <<std::endl;
      std::cout << "final betaY:" << betaY <<std::endl;
      std::cout << "final sigmaY:" << sigmaY <<std::endl;
      std::cout << "\nfinal sigmaZ:" <<  sigmaZ <<std::endl;
      std::cout << "final weightY:" << weightY <<std::endl;
      std::cout << "final weightZ:" << weightZ <<std::endl;

      std::cout << "Average Error for loop :" << loopCount;
      std::cout << "\t is : " << avgError << std::endl;

      // for (size_t j = 0; j < 9; j++) {
      //   std::cout << "y for maturity: "  << tau[j] << "\t is: \t" << crrntMonthMdlData[j] << std::endl;
      // }







  }// DE::runDE


  /****************************************************************************/
  /******************** DE for GDP ********************************************/
  /****************************************************************************/
  void risklabDE::runGDPDE()
  {
  /****************************************************************************/
  /******************** STEP 1 : Initialize *********************************/
  /****************************************************************************/
    // Select the DE Parameters as follows, NP  : Population Size >= 4
    //                                      F   : Scale Factor
    //                                      CR  : Crossover Ratio [0,1]

    const int NP = 15;
    double F = 0.7;
    double CR = 0.6;

    // Creat a population matrix P with the size of [NP * mpCount]
    // where mpCount is the count of Model Parameters;
    // for vasicek it is 3 for alpha, beta and sigma
    // for GDP it is 2 : alphaZ, betaZ
    int mpCount = 2;

    std::vector < std::vector <double> > P(NP,std::vector<double> (mpCount,0));

    // Define the Random Generator here
    // to get random values for model Parameters e.g. alpha, beta, sigma
    // Upper and Lower bounds for them are also defined here
    std::array<double, 2> upperBound = {1.0, 0.05};
    std::array<double, 2> lowerBound = {0.0, 0.00001};

    // Pick Random Variables for model parameters
    // P[i][0] = a random double value for alpha with restrictions 0<alpha<12
    // P[i][1] = a random double value for beta with restrictions beta in R

    // Define the Random Device
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<long double> alphaRands(lowerBound[0],upperBound[0]);
    std::uniform_real_distribution<long double> betaRands(lowerBound[1],upperBound[1]);
    // start the calculation time here

    for(int i = 0; i < NP; i++)
    {
      P[i][0] = alphaRands(gen);
      P[i][1] = betaRands(gen);
    }

    // Define Tolerance for Error
    double tol = 0.00001;
    gdpError = 1.0;
    int maxIter = 10;
    int iter = 0;
    loopCount = 0;

    // Initialize z0 to a given value;
    double z0 = 0.0006;
    auto r = new risklab(z0);

  /****************************************************************************/
  /******************** STEP 2 : DE LOOP **************************************/
  /****************************************************************************/


    while (tol < gdpError && iter < maxIter)
    {
        // Calculate the Vasicek/risklab Error for each of these populations
        // input alpha, beta, sigma; output : error;

        double pError [NP];
        double sum = 0.0;
        for(int i = 0; i < NP; i++)

        {

            r->setGDPParameters(P[i][0], P[i][1]);
            r->setMrktGDP(MrktGDP);
            r->getGDP();
            pError[i] = r->getGDPError();
            sum += pError[i];
        }

        // compute the average Error
        gdpError = sum/NP;
        loopCount++;

        std::cout << "Average Error for GDP loop :" << loopCount;
        std::cout << "\t is : " << gdpError << std::endl;

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
            r->setGDPParameters(crP[i][0], crP[i][1]);
            r->setMrktGDP(MrktGDP);
            r->getGDP();
            crError[i] = r->getGDPError();
            // crError[i] = v->run(crP[i][0], crP[i][1], crP[i][2], crrntMonthMrktData);
        }

        // Now you can compare the Error and if the error of one crossover population
        // is less than the error of one original population you copy that
        for(int i = 0; i < NP; i++)
        {
             if (crError[i] < pError [i])
            {
               P[i][0] = crP[i][0];
               P[i][1] = crP[i][1];
            }
        }
        // So with this new P you can again check the error and go ahead and repeat

        iter++;

    }// end of while loop


    // Print out the final Values and also add them to the main arrays
    double finAlphaZ = 0.0;
    double finBetaZ = 0.0;
    for(int i = 0; i < NP; i++)
    {
      finAlphaZ += P[i][0];
      finBetaZ += P[i][1];
    }
    // Copy them to private values
    alphaZ = finAlphaZ/NP;
    betaZ = finBetaZ/NP;


      std::cout << "\nfinal alphaZ:" <<  alphaZ <<std::endl;
      std::cout << "final betaZ:" << betaZ <<std::endl;
      std::cout << "Average Error for loop :" << loopCount;
      std::cout << "\t is : " << gdpError << std::endl;
      std::cout << "final GDP:" << r->getTheGDP() <<std::endl;


  }// DE::runDE






/****************************************************************************/
/******************** Setters and Getters are here **************************/
/****************************************************************************/

  risklabDE::risklabDE()
  {
    // methodName = mName;
  }

  const double& risklabDE::getAlphaS() const { return alphaS; }
  const double& risklabDE::getBetaS() const { return betaS; }
  const double& risklabDE::getSigmaS() const { return sigmaS; }
  const double& risklabDE::getAlphaY() const { return alphaY; }
  const double& risklabDE::getBetaY() const { return betaY; }
  const double& risklabDE::getSigmaY() const { return sigmaY; }
  const double& risklabDE::getAlphaZ() const { return alphaZ; }
  const double& risklabDE::getBetaZ() const { return betaZ; }
  const double& risklabDE::getSigmaZ() const { return sigmaZ; }
  const double& risklabDE::getWeightY() const { return weightY; }
  const double& risklabDE::getWeightZ() const { return weightZ; }


  const double& risklabDE::getError() const { return avgError; }
  const double& risklabDE::getGDPError() const { return gdpError; }
  const int& risklabDE::getIter() const { return loopCount; }
  const double& risklabDE::getTime() const { return calTime; }

  const std::array<double, 9>& risklabDE::getMdlArray() const
  {
    return crrntMonthMdlData;
  }

  void risklabDE::setMrktArray(std::array<double, 9> const& mrktData)
  {
    crrntMonthMrktData = mrktData;
  }

  void risklabDE::setMrktGDP(double const& mrktData)
	{
		MrktGDP = mrktData;
	}
}// namespace Calibration
