<!-- README.md is generated from README.Rmd. Please edit that file -->
Familias 2.5
============

Familias has existed since 2000 as a Windows program and can be downloaded from <https://familias.no>. The program calculates likelihoods and probabilities used to infer family relationships based on autosomal marker data. The core code of the Windows program has been available since 2012 in R. The current 2.4 cran version <https://cran.r-project.org/package=Familias>, loads the R library 'paramlink'. The present version 2.5 GitHub version does not load 'paramlink' as this library will not be maintained. Rather we recommend Vigeland's new pedtools-suite <https://github.com/magnusdv/pedtools> for creating, manipulating and visualizing pedigrees with or without marker data. The modifications needed to make 'Familias' independent of 'paramlink' have been small. Basically an obsolete function 'ConvertPed' has been removed as has some examples. In addition, the documentation has been slightly updated for this version.

Installation
------------

Install from GitHub by as follows:

``` r
 # First install devtools if needed
if(!require(devtools)) install.packages("devtools")

# Install Familias from GitHub
devtools::install_github("thoree/Familias")
```
