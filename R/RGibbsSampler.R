

#' Gibbs Sampler implemented in R
#' 
#' @param X n x 2 matrix of data
#' @param Sigma known covariance matrix
#' @param iter_max maximum number of iterations
#' @param warm_up number of iterations to warm up for
#' @param verbose print sampling messages 
#' @param seed random number seed
#'
#' @export
#'
RGibbsSampler <- function(X,Sigma,iter_max = 1000,warm_up = 500, verbose = FALSE, seed = NULL){
  
  
  if(!is.null(seed))
    set.seed(seed)
  

  samples <- matrix(0,nrow = iter_max - warm_up, ncol = 2)


  ## Calculate xbar_num | data
  xbar_1 <- mean(X[,1])
  xbar_2 <- mean(X[,2])

  ## initialize  mu_num
  mu_1 <- runif(1,-2,2)
  mu_2 <- runif(1,-2,2)

  sample_ix <- 1 ## to make storage easier

  n <- nrow(X)


  for(iter_ix in 1:iter_max){
	  if(verbose)
		  print(paste0("Sampling iteration: ",iter_ix ,"/" , iter_max))

	  cond_mean_1 <- xbar_1 + Sigma[1,2] * Sigma[2,2]^-1 * (mu_2 - xbar_2)
	  cond_var_1 <- Sigma[1,1] - (Sigma[1,2]^2) * Sigma[2,2]^-1
	  cond_var_1 <- cond_var_1 / n

	  mu_1 <- rnorm(1) * sqrt(cond_var_1) + cond_mean_1

	  cond_mean_2 <- xbar_2 + Sigma[1,2] * Sigma[1,1]^(-1)*(mu_1 - xbar_1)
	  cond_var_2 <- Sigma[2,2] - Sigma[1,2]^2 * Sigma[1,1]^-1
	  cond_var_2 <- cond_var_2 / n

	  mu_2 <- rnorm(1) * sqrt(cond_var_2) + cond_mean_2

	  if(iter_ix > warm_up){
		  samples[sample_ix,1] <- mu_1
		  samples[sample_ix,2] <- mu_2
		  sample_ix <- sample_ix + 1
	  }

  }

  
  return(list(mu_samples = samples))
  
}
