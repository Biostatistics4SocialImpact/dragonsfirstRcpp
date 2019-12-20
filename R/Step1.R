# Demo 1: Inline CPP Function ---------------------------------------------------------


## Example taken from http://adv-r.had.co.nz/Rcpp.html
##

Rcpp::cppFunction('
int add(int x, int y, int z) {
  int sum = x + y + z;
  return sum;
}'
)
# add works like a regular R function
add
#> function (x, y, z) 
#> .Call(<pointer: 0x7f96ecb3ef20>, x, y, z)
add(1,2,3) 

#> [1] 6
