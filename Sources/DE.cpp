#include "../Headers/Vasicek.h"
#include "../Headers/DE.h"

namespace Calibration
{
  #define boundaryMIN 0.000001;

  void DE::runDE()
  {
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
    int mpCount = 3;
    // if (methodName == "risklab")  mpCount = 9;
    // std::array< std::array <double, mpCount> , NP > P;
    std::vector < std::vector <double> > P(NP,std::vector<double> (mpCount,0));

    // man meti ghazal to razi
    // payam elahe pedram
    // raoofi ashkan
    // nokhod loobia goosht ostekhoon sib zamani goje limo amani
    // rob e goje noon lavash sabzi khordan torobche
    // piaz torshi litte
    // ghablame bozorg

    // TODO: right now NP and mpcount cannot be private variables because they
    // are defining the array and the array cannot get non const
    // this will be fixed later that we do vectorization and shit

    // Define the Random Generator here
    // to get random values for model Parameters e.g. alpha, beta, sigma
    // Upper and Lower bounds for them are also defined here
    std::array<double, 3> upperBound = {12.0, 0.05, 1};
    std::array<double, 3> lowerBound = {0.0, 0.00001, 0.00001};

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

    // start the calculation time here
    auto start = std::chrono::steady_clock::now();

    for(int i = 0; i < NP; i++)
    {
      P[i][0] = alphaRands(gen);
      P[i][1] = betaRands(gen);
      P[i][2] = sigmaRands(gen);
    }

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
    auto v = new Vasicek(r0, tau);

/****************************************************************************/
/******************** STEP 2 : DE LOOP **************************************/
/****************************************************************************/


    while (tol < avgError && iter < maxIter)
    {
        // Calculate the Vasicek/risklab Error for each of these populations
        // input alpha, beta, sigma; output : error;

        double pError [NP];
        double sum = 0.0;
        for(int i = 0; i < NP; i++)

        {

            v->setParameters(P[i][0], P[i][1], P[i][2]);
            v->setMrktArray(crrntMonthMrktData);
            v->run();
            pError[i] = v->getError();
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
            v->setParameters(crP[i][0], crP[i][1], crP[i][2]);
            v->setMrktArray(crrntMonthMrktData);
            v->run();
            crError[i] = v->getError();
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
               P[i][2] = crP[i][2];
            }
        }
        // So with this new P you can again check the error and go ahead and repeat

        iter++;

    }// end of while loop


    // Print out the final Values and also add them to the main arrays
    double finAlpha = 0.0;
    double finBeta = 0.0;
    double finSigma = 0.0;
    for(int i = 0; i < NP; i++)
    {
      finAlpha += P[i][0];
      finBeta += P[i][1];
      finSigma += P[i][2];
    }
    // Copy them to private values
    alpha = finAlpha/NP;
    beta = finBeta/NP;
    sigma = finSigma/NP;

  /****************************************************************************/
  /***************** STEP 4 : Final Calculation *******************************/
  /****************************************************************************/
    //get the final values with final parameters
    double r1 = v->nextRate();
    for (size_t i = 0; i < 9; i++) {
      crrntMonthMdlData[i] = v->getYield(r1, tau[i]);
    }

    // end the Calculation time here
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> durationCount = end - start;
    calTime = durationCount.count();
  /****************************************************************************/
	/*************************** STEP 4 : Print Out *****************************/
	/****************************************************************************/

      std::cout << "\nfinal alpha:" <<  alpha <<std::endl;
      std::cout << "final beta:" << beta <<std::endl;
      std::cout << "final sigma:" << sigma <<std::endl;
      std::cout << "Average Error for loop :" << loopCount;
      std::cout << "\t is : " << avgError << std::endl;

      for (size_t j = 0; j < 9; j++) {
        std::cout << "y for maturity: "  << tau[j] << "\t is: \t" << crrntMonthMdlData[j] << std::endl;
      }







  }// DE::runDE

/****************************************************************************/
/******************** Setters and Getters are here **************************/
/****************************************************************************/

  DE::DE(std::string const& mName)
  {
    methodName = mName;
  }

  const double& DE::getAlpha() const { return alpha; }
  const double& DE::getBeta() const { return beta; }
  const double& DE::getSigma() const { return sigma; }
  const double& DE::getError() const { return avgError; }
  const int& DE::getIter() const { return loopCount; }
  const double& DE::getTime() const { return calTime; }

  const std::array<double, 9>& DE::getMdlArray() const
  {
    return crrntMonthMdlData;
  }

  void DE::setMrktArray(std::array<double, 9> const& mrktData)
  {
    crrntMonthMrktData = mrktData;
  }

}// namespace Calibration
