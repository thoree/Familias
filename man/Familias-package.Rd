\name{Familias-package}
\alias{Familias-package}
\alias{Familias}
\docType{package}
\title{Familias: Inferring paternity and indentification based on DNA data
}
\description{
Familias has existed for a number of years as a Windows program for calculating
probabilities in connection with using autosomal marker DNA data to infer family relationships. 
The core code of the Windows program, available from
\url{https://familias.no}, is contained in this R package. 
}
\details{
This package represents an implementation of an interface to the 
core Familias functions, which are programmed in C++. 
The previous version, 2.4., loaded the R library paramlink for some examples.
The present version does not load paramlink. Rather we recommend 
the pedtools-suite, see \url{https://github.com/magnusdv/pedtools}
for creating, manipulating and visualizing pedigrees with or without marker data.
}
\author{
Petter Mostad <mostad@chalmers.se> and Thore Egeland <Thore.Egeland@nmbu.no>
}
\references{
For more information, see \url{https://familias.name/}.
}
\keyword{ package }
\examples{
persons <- c("mother", "daughter", "AF")
ped1 <- FamiliasPedigree(id = persons, dadid = c(NA, "AF", NA), 
momid <- c(NA, "mother", NA), 
sex <- c("female", "female", "male"))
ped2 <- FamiliasPedigree(id = c(persons, "TF"), dadid = c(NA, "TF", NA, NA), 
momid <- c(NA, "mother", NA, NA), 
sex=c("female", "female", "male", "male"))
ped3 <- FamiliasPedigree(id = c(persons, "TF", "gf", "gm"), 
dadid <- c(NA, "TF", "gf", "gf", NA, NA), 
momid <- c(NA, "mother", "gm", "gm", NA, NA), 
sex <- c("female", "female", "male", "male", "male", "female"))
# par(mfrow = c(3,1))
# plot(ped1);title("ped1, i.e., AF is father")
# plot(ped2);title("ped2, i.e., AF is unrelated")
# plot(ped3);title("ped3, i.e., AF is uncle")
# par(mfrow = c(1,1))
mypedigrees <- list(isFather = ped1, unrelated=ped2, isUncle = ped3)
locus1 <- FamiliasLocus(frequencies = c(0.1, 0.2, 0.3, 0.4), 
allelenames <- c("A", "B", "C", "D"), name="locus1")
locus2 <- FamiliasLocus(c(0.2, 0.3, 0.5), c(17, 18, 19), "loc2",  
femaleMutationRate = 0.05)
myloci <- list(locus1, locus2)
datamatrix <- data.frame(locus1.1 = c("A", "A", "A"), locus1.2 = c("B", "B", "C"), 
locus2.1 = c(17, 19, 19), locus2.2=c(18, 18, 18))
rownames(datamatrix) <- persons
FamiliasPosterior(mypedigrees, myloci, datamatrix)
}
