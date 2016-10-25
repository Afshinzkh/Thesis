#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <array>
#include <random>


#include "vasicekMain.cpp"

using namespace Vasicek;



int main()
{
	// Select the DE Parameters as follows, NP  : Population Size >= 4
    //                                      F   : Scale Factor
	//                                      CR  : Crossover Ratio [0,1]

    const int NP = 15;
    double F = 0.9;
    double CR = 0.6;

    // Creat a population matrix P with the size of [NP * mpCount]
    // where mpCount is the count of Model Parameters;
    // e.g. for vasicek it is 3 for alpha, beta and sigma
    const int mpCount = 3;
    std::array< std::array <double, mpCount> , NP > P;

    // Define the Random Generator here
    // to get random values for model Parameters e.g. alpha, beta, sigma
    // Upper and Lower bounds for them are also defined here
    std::array<double, 3> upperBound = {12.0, 1000000, 1000000};
    std::array<double, 3> lowerBound = {0.0, -1000000, 0.0};

    std::random_device rd;
	 	std::mt19937 gen(rd());
	 	std::uniform_real_distribution<> alphaRands(lowerBound[0],upperBound[0]);
    std::uniform_real_distribution<> betaRands(lowerBound[1],upperBound[1]);
    std::uniform_real_distribution<> sigmaRands(lowerBound[2],upperBound[2]);

    // Pick Random Variables for model parameters
    // P[i][0] = a random double value for alpha with restrictions 0<alpha<12
    // P[i][1] = a random double value for beta with restrictions beta in R
    // P[i][2] = a random double value for sigma with restrictions 0<sigma

    for(int i = 0; i < NP; i++)
    {
      P[i][0] = alphaRands(gen);
      P[i][1] = betaRands(gen);
      P[i][2] = sigmaRands(gen);
    }

    // for(int i = 0; i < NP; i++)
    // {
    //   std::cout << "random alpha " << i+1 << " is: " << P[i][0] <<std::endl;
    //   std::cout << "random beta " << i+1 << " is: " << P[i][1] <<std::endl;
    //   std::cout << "random sigma " << i+1 << " is: " << P[i][2] <<std::endl;
    // }

    // Define Tolerance for Error
    double tol = 0.1;
    double avgError = 1.0;

    int loopCount = 0;
    // The loop of DE Starts Here
    while (tol < avgError)
    {
        // Calculate the Vasicek/risklab Error for each of these populations
        // input alpha, beta, sigma; output : error;

        double pError [NP];
        double sum = 0.0;
        for(int i = 0; i < NP; i++)
        {
            pError[i] = vasicekMode(P[i][0], P[i][1], P[i][2]);
            sum += pError[i];
        }

        // compute the average Error
        avgError = sum/NP;
        loopCount++;
        std::cout << "Average Error for Calculation loop :" << loopCount;
        std::cout << "\t is : " << avgError << std::endl;
        if(avgError < tol)
        {

          double finAlpha, finBeta, finSigma;
          for(int i = 0; i < NP; i++)
          {
            finAlpha += P[i][0];
            finBeta += P[i][1];
            finSigma += P[i][2];
          }
          std::cout << "final alpha:" << finAlpha/NP <<std::endl;
          std::cout << "final beta:" << finBeta/NP <<std::endl;
          std::cout << "final sigma:" << finSigma/NP <<std::endl;
        }
        // std::cout << "Press Enter" << std::endl;
        // getchar();

        // Do the MUtation Stage as follows
        // define a mutated population as mutP
        std::array< std::array <double, mpCount> , NP > mutP;

        // TODO:  Here the boundaries of the model parameters might exceed,
        //  so you should think of something
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

            // std::cout << "Created Indexes are: " << i << " " << id1 << id2 << id3 <<std::endl;

            mutP[i][0] = P[id1][0] + F * (P[id2][0] - P[id3][0]);
            if(mutP[i][0] > upperBound[0])
                mutP[i][0] = upperBound[0] - 0.00001;
            if(mutP[i][0] < lowerBound[0])
                mutP[i][0] = lowerBound[0] + 0.00001;

            mutP[i][1] = P[id1][1] + F * (P[id2][1] - P[id3][1]);
            if(mutP[i][1] > upperBound[1])
                mutP[i][1] = upperBound[1] - 0.00001;
            if(mutP[i][1] < lowerBound[1])
                mutP[i][1] = lowerBound[1] + 0.00001;

            mutP[i][2] = P[id1][2] + F * (P[id2][2] - P[id3][2]);
            if(mutP[i][2] > upperBound[2])
                mutP[i][2] = upperBound[2] - 0.00001;
            if(mutP[i][2] < lowerBound[2])
                mutP[i][2] = lowerBound[2] + 0.00001;

        }

        // Crossover the mutated with the origital Poupation
        // define a Crossover Population as crP
        std::array< std::array <double, mpCount> , NP > crP;

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
            crError[i] = vasicekMode(crP[i][0], crP[i][1], crP[i][2]);
        }

        // Now you can compare the Error and if the error of one crossover population
        // is less than the error of one original population you copy that
        // sum = 0.0;
        for(int i = 0; i < NP; i++)
        {
             if (crError[i] < pError [i])
            {
               P[i][0] = crP[i][0];
               P[i][1] = crP[i][1];
               P[i][2] = crP[i][2];
              //  sum += crError[i];
            }
            // else
            //   sum += pError[i];
        }
        // double newError = sum/NP;
        // // So with this new P you can again check the error and go ahead and repeat
        // // but before check if we are stuck
        // if (std::abs(newError - avgError) < 0.00001){
        //   std::cout << "Average Error for Calculation loop :" << loopCount+1;
        //   std::cout << "\t is : " << avgError << std::endl;
        //   std::cout << "no good result :( " << std::endl;
        //   break;
        // }

    }

    // TODO: get return values from deMain as a double array
}
