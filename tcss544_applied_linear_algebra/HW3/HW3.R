####################################################
# Jesse Bannon
# 2017-04-23
# Applied Linear Algebra HW3
####################################################

# Let G be an adjacency matrix of hyperlinks
# G_i,j represents a hyperlink from j -> i
#
# p probability that a random walk follows a link
# 1-p probability that some arbitrary page is chosen
page_rank <- function(G, p) {
  n <- nrow(G)
  
  # The probability that a particular random page is chosen
  delta <- (1 - p) / n
  
  # number of links on j
  col_sums <- colSums(G)
  
  # Let A be an n x n matrix whose elements are
  # a_ij = { p * g_ij / c_j + delta : c_j != 0 }
  #        { 1/n                    : c_j == 0 }
  #
  # A = pGD + e*t(z) where
  # D = diag(1/c_j)
  # z = { delta : c_j != 0 }
  #     { 1/n   : c_j == 0 }
  # 
  # We solve
  # Ax = x
  # 0 = x - Ax
  # 0 = (I - A)x
  # 0 = (I - (pGD + e*t(z)))x
  # 0 = (I - pGD)x - t(z)x
  # t(z)x = (I - pGD)x
  #
  # We don't know t(z)x, we can temporarily take e (each element as 1).
  # As long as p is strictly less than one, the coefficient matrix I - pGD is non-singular
  # and (I - pGD)x = e can be solved.
  D <- diag(1/col_sums, nrow = n, ncol = n)
  for (i in 1:n) {
    if (!is.finite(D[i,i])) {
      D[i,i] <- 0
    }
  }
  
  z <- col_sums * (1 / n)
  e <- rep(1, n)
  
  x <- solve(diag(n) - (p * (G %*% D)), e)
  x <- x / sum(x)
  return(x)
}

####################################################
# Excercise 7.6
####################################################

# What is the connectivity matrix of G
#
# @returns
#      [,1] [,2] [,3] [,4] [,5] [,6]
# [1,]    0    0    1    0    0    0
# [2,]    1    0    0    0    0    0
# [3,]    0    1    0    1    0    0
# [4,]    1    1    0    0    0    0
# [5,]    0    0    0    0    0    1
# [6,]    0    0    0    0    1    0
excercise_7a <- function() {
  G <- matrix(data = 0, nrow=6, ncol=6)
  a <- 1 # alpha
  b <- 2 # beta
  d <- 3 # delta
  g <- 4 # gamma
  r <- 5 # rho
  s <- 6 # sigma
  
  # G[i,j] = { 1 : Link from j to i    }
  #          { 0 : No link from j to i }
  
  # To alpha from j
  G[a, d] <- 1
  
  # To beta from j
  G[b, a] <- 1
  
  # To delta from j
  G[d, b] <- 1
  G[d, g] <- 1
  
  # To gamma from j
  G[g, a] <- 1
  G[g, b] <- 1
  
  # To rho from j
  G[r, s] <- 1
  
  # To sigma from j
  G[s, r] <- 1
  
  return(G)
}

# What are the PageRanks if the hyperlink transition probability p = 0.85
#
# @returns (0.1981398 0.1092094 0.2036939 0.1556234 0.1666667 0.1666667)
excercise_7b <- function() {
  return(page_rank(excercise_7a(), 0.85))
}

# Describe what happens to both the definition of PageRank and the computation done with probability p -> 1
#
# If p -> 1, we have the following Markov Matrix A
#
# A_ij = { g_ij / c_j : c_j != 0 }
#        { 1/n        : c_j == 0 }
#
# 
# p -> 1 implies that no one will ever go to an arbitrary page, rather, 
# they will only follow a link if it's on their current page j.
#
# If there is a link from j to i, there is 1/c_j chance you will hop from j to i
#
# The PageRank output implies that starting at any arbitrary webpage j, over time
# continuously traversing to links i on your current page j' shows the probability
# of ending up at webpage i
excercise_7c <- function() {
  return(page_rank(excercise_7a(), 0.99999))
}

####################################################
# Excercise 7.8
####################################################

# Maybe 1/2 a point for my final grade? :)
power_iter_page_rank <- function(G, p, epsilon) {
  n <- nrow(G)
  A <- (1 - p) / n
  delta <- (1 - p) / n
  indices <- vector("list", n)
  col_sums <- vector(mode = "double", length = n)
  
  for (j in 1:n) {
    indices[[j]] <- which(G[,j] == 1)
    col_sums[j] <- length(indices[[j]])
  }
  
  x <- rep(1/n, n)
  z <- rep(0, n)
  
  while(max(abs(x - z)) > epsilon) {
    z <- x
    x <- rep(0, n)
    
    for (j in 1:n) {
      if (col_sums[j] == 0) {
        x <- x + z[j] / n
      } else {
        x[indices[[j]]] <- x[indices[[j]]] + z[j] / col_sums[j]
      }
    }
    x <- p*x + delta
  }
  
  return(x)
}

####################################################
# TEST
####################################################

page_rank_test <- function() {
  moler_example <- t(matrix(c(0, 0, 0, 1, 0, 1,
                              1, 0, 0, 0, 0, 0,
                              0, 1, 0, 0, 0, 0,
                              0, 1, 1, 0, 0, 0,
                              0, 0, 1, 0, 0, 0,
                              1, 0, 1, 0, 0, 0), 6, 6))
  output <- page_rank(moler_example, 0.85)
  expectedOutput <- c(.3210, .1705, .1066, .1368, .0643, .2007)
  
  return(max(abs(output - expectedOutput)) < .001)
}

power_iter_page_rank_test <- function() {
  moler_example <- t(matrix(c(0, 0, 0, 1, 0, 1,
                              1, 0, 0, 0, 0, 0,
                              0, 1, 0, 0, 0, 0,
                              0, 1, 1, 0, 0, 0,
                              0, 0, 1, 0, 0, 0,
                              1, 0, 1, 0, 0, 0), 6, 6))
  output <- power_iter_page_rank(moler_example, 0.85, .0001)
  expectedOutput <- c(.3210, .1705, .1066, .1368, .0643, .2007)
  
  return(max(abs(output - expectedOutput)) < .001)
}

printTestResult <- function(testName, testResult) {
  output <- if (testResult) "PASS" else "FAIL"
  cat(sprintf("%s: %s\n", testName, output))
}

printTestResult("Page Rank Test", page_rank_test())
printTestResult("Power Iter Page Rank Test", power_iter_page_rank_test())
