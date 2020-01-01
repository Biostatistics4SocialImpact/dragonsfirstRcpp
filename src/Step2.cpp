#include <Rcpp.h>
#include <cmath> // for math functions
#include <random> // for random numbers
using namespace Rcpp;

// This is a simple example of exporting a C++ function to R. You can
// source this function into an R session using the Rcpp::sourceCpp 
// function (or via the Source button on the editor toolbar). Learn
// more about Rcpp at:
//
//   http://www.rcpp.org/
//   http://adv-r.had.co.nz/Rcpp.html
//   http://gallery.rcpp.org/
//
//
//

void print_progress(const int &iter_ix,const int &warm_up,const int &iter_max){

  if(iter_max > 20){
      if((iter_ix) % (int)round(.1 * iter_max) == 0 || iter_ix == 1 || iter_ix == (warm_up + 1) ){
          int progress = (int)round(iter_ix * 100 / iter_max);
          std::string str = (iter_ix) <= (warm_up) ? "\t [Warmup]" : "\t [Sampling]";
          Rcpp::Rcout << "Beginning of iteration: " << (iter_ix) << " / " << iter_max << " (" << progress << "%)" << str  << std::endl;
      }
  }
  else{
          int progress = (int)round(iter_ix * 100 / iter_max);
          std::string str = (iter_ix) <= (warm_up) ? "\t [Warmup]" : "\t [Sampling]";
          Rcpp::Rcout <<  "Beginning of iteration: " << (iter_ix) << " / " << iter_max << " (" << progress << "%)" << str  << std::endl;
  }

}


//' GibbsSampler for Bivariate Normal Data with known Covariance Matrix
//' 
//'
//' @param X n x 2 matrix  of sample data
//' @param Sigma 2x2 known covariance matrix
//' @param iter_max integer maximum number of iterations
//' @param warm_up integer number of iterations to warm_up the chain
//' @param verbose boolean that indicates whether to print progress messages
//' @param seed random number generator seed
//'
// [[Rcpp::export]] 
List GibbsSampler(NumericMatrix X,
                  NumericMatrix Sigma,
                  const int iter_max,
                  const int warm_up,
				  const bool verbose,
                  const int seed) {
   
	std::mt19937 rng; // random number generator object
	rng = std::mt19937(seed); // initialized random number generator object
	std::normal_distribution<double> rnorm(0,1); // initialized standard normal number generator
	std::uniform_real_distribution<double> runif(-2,2);
   
	// create storage space
	NumericMatrix mu_samples(iter_max - warm_up,X.cols());

	// calculate xbar_num | data
	const double xbar_1 = mean(X(_,0)); // use the const keyword, because these won't change
	const double xbar_2 = mean(X(_,1)); // C++ has zero-indexed  data structures

	 // initialize mu_num
	 double mu_1 = runif(rng); // we don't use const here, because these *will* change
	 double mu_2 = runif(rng);

	 const int n = X.nrow();

	 // create conditional dist'n variables 
	 
	 double cond_mean_1;
	 double cond_mean_2;

	 double cond_var_1;
	 double cond_var_2;

	 // to make storage easier
	 double sample_ix = 0;
	 
   
   //begin sampling
	for(int iter_ix = 1; iter_ix <= iter_max; iter_ix ++){
		if(verbose)
			print_progress(iter_ix,warm_up, iter_max);
	 
	 
	 // calculate mu_1 conditional mean,variance
	 cond_mean_1 = xbar_1 + Sigma(0,1) * pow(Sigma(1,1),-1) * (mu_2 - xbar_2);
	 cond_var_1 = Sigma(0,0) - pow(Sigma(0,1),2) * pow(Sigma(1,1),-1);
	 cond_var_1 = cond_var_1 / n;

	 // draw mu_1
	 mu_1 = rnorm(rng) * sqrt(cond_var_1) + cond_mean_1;
	 
	 // calculate mu_2 conditional mean, variance
	 cond_mean_2 = xbar_2 + Sigma(0,1) * pow(Sigma(0,0),-1) * (mu_1 - xbar_1);
	 cond_var_2 = Sigma(1,1) - pow(Sigma(0,1),2) * pow(Sigma(0,0),-1);
	 cond_var_2 = cond_var_2 / n;

	 // draw mu_2
	 mu_2 = rnorm(rng) * sqrt(cond_var_2) + cond_mean_2;

	 
	 // if the iteration is after warm-up, then store the samples
	 //
	 if(iter_ix > warm_up){
		 mu_samples(sample_ix,0) = mu_1;
		 mu_samples(sample_ix,1) = mu_2;
		 sample_ix ++;
	 }
	 
	}
   
   
	return(List::create((Named("mu_samples") = mu_samples)));
}

