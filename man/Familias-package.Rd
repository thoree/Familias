\name{Familias-package}
\alias{Familias-package}
\alias{Familias}
\docType{package}
\title{Familias: Inferring paternity and identification based on DNA data
}
\description{
This package provides an R interface to the Windows program Familias 
(\url{https://familias.no}), for calculating probabilities in forensic family 
genetics. The core source code (in C++) of the Windows program is contained in 
the package.
}
\details{
The main purpose of this package is to provide an R interface to the calculation 
of likelihood ratios (LRs) in Familias. For other forensic pedigree analyses and 
visualisations we recommend the `pedsuite` packages: 
\url{https://magnusdv.github.io/pedsuite/}. In particular the package 
`pedFamilias` facilitates conversion of .fam files into the `pedsuite` format.
}
\author{
Petter Mostad <mostad@chalmers.se> and Thore Egeland <Thore.Egeland@nmbu.no>.
}
\references{
For more information, see \url{https://familias.name/}.
}
\keyword{ package }
\examples{
persons <- c("mother", "daughter", "AF")

# Pedigrees
ped1 <- FamiliasPedigree(id = persons, 
                         dadid = c(NA, "AF", NA), 
                         momid = c(NA, "mother", NA), 
                         sex = c("female", "female", "male"))
                         
ped2 <- FamiliasPedigree(id = c(persons, "TF"), 
                         dadid = c(NA, "TF", NA, NA), 
                         momid = c(NA, "mother", NA, NA), 
                         sex = c("female", "female", "male", "male"))
                         
ped3 <- FamiliasPedigree(id = c(persons, "TF", "gf", "gm"), 
                         dadid = c(NA, "TF", "gf", "gf", NA, NA), 
                         momid = c(NA, "mother", "gm", "gm", NA, NA), 
                         sex = c("female", "female", "male", "male", "male", "female"))
op = par(mfrow = c(1,3))
plot(ped1); title("ped1: AF is father")
plot(ped2); title("ped2: AF is unrelated")
plot(ped3); title("ped3: AF is uncle")
par(op)

mypedigrees <- list(isFather = ped1, unrelated=ped2, isUncle = ped3)

# Loci
locus1 <- FamiliasLocus(frequencies = c(0.1, 0.2, 0.3, 0.4), 
                        allelenames = c("A", "B", "C", "D"), 
                        name = "locus1")
locus2 <- FamiliasLocus(c(0.2, 0.3, 0.5), 
                        c(17, 18, 19), 
                        "loc2",
                        femaleMutationRate = 0.05)
myloci <- list(locus1, locus2)

# Genotype data
datamatrix <- data.frame(locus1.1 = c("A", "A", "A"), 
                         locus1.2 = c("B", "B", "C"), 
                         locus2.1 = c(17, 19, 19), 
                         locus2.2 = c(18, 18, 18), 
                         row.names = persons)

# Calculate LR
FamiliasPosterior(mypedigrees, myloci, datamatrix)
}
