#include<iostream>
#include<array>// make a matrix class 
#include <stdlib.h> 

using namespace std;

int main()
{
	// Select the DE Parameters as follows, NP  : Population Size >= 4
    //                                      F   : Scale Factor
	//                                      CR  : Crossover Ratio [0,1]

    int NP = 5;
    double F = 0.5;
    double CR = 0.5;

    // Creat a population matrix P with the size of [NP * mpCount]
    // where mpCount is the count of Model Parameters;
    // e.g. for vasicek it is 3 for alpha, beta and sigma
    int mpCount = 3;
    double P[NP * mpCount];
    for(int i = 0; i < NP; i++)
    {
            // P[i][0] = a random double value for alpha with restrictions 0<alpha<12
            // P[i][1] = a random double value for beta with restrictions beta in R
            // P[i][2] = a random double value for sigma with restrictions 0<sigma
    }
    
    // Define Tolerance for Error
    double tol = 0.00001;
    double avgError = 1.0;

    // The loop of DE Starts Here
    while (tol < avgError)
    {
        // Calculate the Vasicek/risklab Error for each of these populations
        // input alpha, beta, sigma; output : error;
        
        double pError [NP];
        for(int i = 0; i < NP; i++)
        {
            // pError[i] = VasicekMode(P[i,0], P[i,1], P[i,2])
        }

        // compute the average Error
        double sum = 0.0;
        for(int i = 0; i < NP; i++)
        {
            //sum += pError[i];
        }
        avgError = sum/NP;


        // Do the MUtation Stage as follows
        // define a mutated population as mutP
        double mutP[NP * mpCount];

        /* THINK*/
        // Here the boundaries of the model parameters might exceed, so you should think of something
        for (int i = 0; i < NP; i++)
        {
            // create 3 random indexes for each i the new mutated population set
            // with restriction  0<=id1,id2,id3<NP
            // and all the indexes should be different from each other and also from i

            // mutP[i,0] = P[id1,0] + F * (P[id2,0] - P[id3,0]);
            // mutP[i,1] = P[id1,1] + F * (P[id2,1] - P[id3,1]);
            // mutP[i,2] = P[id1,2] + F * (P[id2,2] - P[id3,2]);
        }

        // Crossover the mutated with the origital Poupation
        // define a Crossover Population as crP
        double crP[NP * mpCount];
        
        // choose each model Parameter  as follows
        for (int i = 0; i < NP; i++)
        {
            for (int j = 0; j < mpCount; j++)
            {
                // if(rand(U[0,1]) < CR || intRand(a random index between 0 to mpCount) == j);
                //     crP[i,j] = mutP[i,j];
                // else
                //     crP[i,j] = P[i,j];
            }
        }

        // Now that we have the Crossover Population we can calculate it's Errors
        double crError [NP];
        for(int i = 0; i < NP; i++)
        {
            // crError[i] = VasicekMode(crP[i,0], crP[i,1], crP[i,2])
        }

        // Now you can compare the Error and if the error of one crossover population
        // is less than the error of one original population you copy that
        for(int i = 0; i < NP; i++)
        {
            // if (crError[i] < pError [i])
            //     {
            //         P[i,0] = crP[i,0];
            //         P[i,1] = crP[i,1];
            //         P[i,2] = crP[i,2]; 
            //     }
            
        }
        // So with this new P you can again check the error and go ahead and repeat 
    }
}