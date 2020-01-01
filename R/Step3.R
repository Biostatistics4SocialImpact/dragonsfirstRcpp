# # Gibbs Sampler in R -----------------------------------------------------------
# # require(MASS)
# # require(tictoc)
# # require(benchmark)
# ## Generate data true_mu = c(3,1) and
# # true_Sigma =
# # [1,]  1.0  0.3
# # [2,]  0.3  1.0
# #
# set.seed(34243)
# S <- matrix(c(1,0.3,0.3,1),nrow=2,ncol=2)
# X <- MASS::mvrnorm(n = 100,mu = c(3,1),Sigma = S)
# 
# # plot(X) ## If you want to visualize the sample
# ## use our Rcpp function
# 
# tictoc::tic()
# fit <- GibbsSampler(X,S,iter_max = 1000,
#                      warm_up = 500,
#                     verbose = TRUE,
#                      seed = 1231)
# tictoc::toc()
# 
# t(apply(fit$mu_samples,2,function(x) quantile(x,c(0.025,0.5,0.975))))
# 
# tictoc::tic()
# fit2 <- RGibbsSampler(X,S,verbose = TRUE,seed = 1231)
# tictoc::toc()
# 
# ## More exact calculation
# 
# tictoc::tic()
# fit <- GibbsSampler(X,S,iter_max = 1000,
#                     warm_up = 500,
#                     verbose = FALSE,
#                     seed = 1231)
# tictoc::toc()
# 
# 
# tictoc::tic()
# fit2 <- RGibbsSampler(X,S,verbose = FALSE,seed = 1231)
# tictoc::toc() # ~ 4 times slower on my computer
# 
