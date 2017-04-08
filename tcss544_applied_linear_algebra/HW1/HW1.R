####################################################
# Jesse Bannon
# 2017-04-07
# Applied Linear Algebra HW1
####################################################

# Requires package 'Matrix' from CRAN
if (!require("Matrix")) {
  install.packages("Matrix")
  library("Matrix")
}

####################################################
# QUESTION 1
####################################################

TriangleArea <- function(a, b, c)
{
  if (a <= 0 || b <= 0 || c <= 0) {
    stop("Invalid side lengths, must be > 0")
  }
  s <- (a + b + c) / 2
  area <- sqrt(s * (s - a) * (s - b) * (s - c))
  return(area)
}

TriangleAreaTest <- function()
{
  area1 <- TriangleArea(3, 4, 5)
  area2 <- TriangleArea(1, 1, 1)
  area3 <- TriangleArea(1, 1, sqrt(2))
  
  epsilon <- .001
  testArea1 <- abs(area1 - 6.0) < epsilon
  testArea2 <- abs(area2 - .433) < epsilon
  testArea3 <- abs(area3 - .5) < epsilon
  
  return(testArea1 && testArea2 && testArea3)
}

####################################################
# QUESTION 2
####################################################

IsDag <- function(A)
{
  powerA <- A
  isDag <- TRUE
  n <- nrow(A)
  p <- 1
  
  while(p <= n && isDag) {
    # If any diagonal element is not 0, it is not a DAG because some vertex i
    # has a path to itself in p number of steps.
    if (any(diag(A) != 0)) {
      isDag <- FALSE
    }
    powerA <- powerA %*% A
    p <- p + 1
  }
  
  # We know a DAG's longest path can only have n - 1 walks
  # If A^n is not 0, then A is not a DAG.
  if (isDag && any(powerA != 0)) {
    isDag <- FALSE
  }
  
  return(isDag)
}

# Includes paths of length zero
TotalPaths <- function(A)
{
  if (!IsDag(A)) {
    return(Inf)
  }
  
  # Sorry, you didn't say it had to be optimized :)
  # Essentially doing the same O(n^4) operation twice
  powerA <- A
  totalPaths <- 0
  while(any(powerA != 0)) {
    totalPaths <- totalPaths + sum(powerA)
    powerA <- powerA %*% A
  }
  
  return(totalPaths)
}

TotalPathsTest <- function()
{
  # Reverse diag matrix
  m1 <- apply(diag(5), 2, rev)
  
  # All possible paths
  m2 <- matrix(1, 5, 5)
  
  # Should contain 5 paths
  m3 <- matrix(0, 5, 5)
  m3[1, 5] <- 1
  m3[3, 5] <- 1
  m3[5, 2] <- 1
  
  test1 <- TotalPaths(m1) == Inf
  test2 <- TotalPaths(m2) == Inf
  test3 <- TotalPaths(m3) == 5
  
  return(test1 && test2 && test3)
}

####################################################
# QUESTION 3
####################################################

# Converts a character to its ASCII integer code
charToASCII <- function(c)
{
  return(strtoi(charToRaw(c),16L))
}

toNormalizedArray <- function(paragraph)
{
  # lowercase entire paragraph
  paragraph <- tolower(paragraph)
  
  # remove everything that's not an alphabet character, space, or dash
  paragraph <- gsub("[^[a-z -]]*", "", paragraph)
  
  # split string on any whitespace
  return(strsplit(paragraph, "\\s")[[1]])
}

# Converted provided MATLAB hash function to R
hash <- function(s, largeprime)
{
  sCharArray <- strsplit(s, "")[[1]]
  fac <- 256
  h <- 0
  for (i in 1:length(sCharArray)) {
    h <- (h * fac + charToASCII(sCharArray[i])) %% largeprime
  }
  h <- h + 1
  return(h)
}

# Converted provided MATLAB stopwords function to R
getStopwords <- function()
{
  return(c("about", "all", "along", "also", "although", "among", "and", "any", 
    "anyone", "anything", "are", "around", "because", "been", "before", 
    "being", "both", "but", "came", "come", "coming", "could", "did", 
    "each", "else", "every", "for", "from", "get", "getting", "going", 
    "got", "gotten", "had", "has", "have", "having", "her", "here", 
    "hers", "him", "his", "how", "however", "into", "its", "like", 
    "may", "most", "next", "now", "only", "our", "out", "particular", 
    "same", "she", "should", "some", "take", "taken", "taking", "than", 
    "that", "the", "then", "there", "these", "they", "this", "those", 
    "throughout", "too", "took", "very", "was", "went", "what", "when", 
    "which", "while", "who", "why", "will", "with", "without", "would", 
    "yes", "yet", "you", "your", "one", "ones", "two", "three", "four", 
    "five", "six", "seven", "eight", "nine", "ten", "tens", "eleven", 
    "twelve", "dozen", "dozens", "thirteen", "fourteen", "fifteen", 
    "sixteen", "seventeen", "eighteen", "nineteen", "twenty", "thirty", 
    "forty", "fifty", "sixty", "seventy", "eighty", "ninety", "hundred", 
    "hundreds", "thousand", "thousands", "million", "millions"))
}

sparseNorm<- function(sVector)
{
  sqrt(sum(sVector^2))
}

FindSimilarity <- function(X, Y)
{
  OddNumber <- 10000001
  stopwords <- getStopwords()
  
  s <- Matrix::sparseVector(0, 0, OddNumber)
  x <- Matrix::sparseVector(0, 0, OddNumber)
  y <- Matrix::sparseVector(0, 0, OddNumber)
  
  allWords <- c(X, Y)
  
  for (word in allWords) {
    idx <- hash(word, OddNumber)
    if (nchar(word) < 3 || word %in% stopwords) {
      s[idx] <- 1
    } else {
      x[idx] <- sum(word == X)
      y[idx] <- sum(word == Y)
    }
  }
  
  return(as.double(x %*% y) / (sparseNorm(x) * sparseNorm(y)))
}

FindSimilarityTest <- function()
{
  X <- c("how", "much", "wood", "could", "a", "woodchuck", "chuck", "if", "a", "woodchuck", "could", "chuck", "wood")
  Y <- c("all", "the", "wood", "that", "a", "woodchuck", "could", "if", "a", "woodchuck", "could", "chuck", "wood")
  similarity <- FindSimilarity(X, Y)
  return(abs(similarity - .9245) < .001)
}

####################################################
# TESTS
####################################################

printTestResult <- function(testName, testResult) {
  output <- if (testResult) "PASS" else "FAIL"
  cat(sprintf("%s: %s\n", testName, output))
}

printTestResult("TriangleArea Test", TriangleAreaTest())
printTestResult("TotalPaths Test", TotalPathsTest())
printTestResult("FindSimilarity Test", FindSimilarityTest())
