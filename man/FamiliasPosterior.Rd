\name{FamiliasPosterior}
\alias{FamiliasPosterior}
\title{
Calculates posterior probabilities and likelihoods for pedigrees
}
\description{
The calculations of the windows version of Familias 2.0
are made available in an R environment.
}
\usage{
FamiliasPosterior(pedigrees, loci, datamatrix, prior, ref = 1, 
kinship = 0, simplifyMutations = FALSE)
}

\arguments{
  \item{pedigrees}{
An object of type 'FamiliasPedigree' or 'pedigree', or a list of such objects
}
  \item{loci}{
A FamiliasLocus object or a list of such objects
}
  \item{datamatrix}{
A data frame. The row names must be the names of the persons you have data for. The columns contain
the alleles, two columns for each marker, in the same order used in the loci list. 
}
  \item{prior}{
A vector of non-negative probabilities summing to 1. As default a flat prior is used, with all values equal. 
}
  \item{ref}{
The index in the list of pedigrees of the pedigree which should be used as reference when computing likelihood ratios. 
The default value is 1. 
}
  \item{kinship}{
A real in [0,1], commonly denoted theta in forensics included to model sub-population effects as departures
from Hardy-Weinberg equilibrium. The default value is zero.
}
  \item{simplifyMutations}{
In pedigrees with several generations multistep mutations may happen. If the probability of mutating to an allele 
depends on which allele it mutates from, exact likelihood computations must keep track of all the possible values of 
mutated alleles in such multistage mutations, and this may slow down computations considerably. Instead, one may 
use in computations for multistage mutations a single allele that is not among those observed in the data. 
When this approach gives exact results it is always used; 
in other cases one may choose to use it as an approximation by setting 
simplifyMutations to TRUE. The properties of the extra allele is the weighted average (by population frequencies) 
of the alleles not observed in the data. 
}
}
\value{
\item{posterior}{probabilities for each pedigree}
\item{prior}{prior returned for convenience and backward compatibility}
\item{LR}{Likelihood ratios using the pedigree indicated with the ref parameter (default value 1) as basis}
\item{LRperMarker}{Likelihood ratios per marker using the pedigree indicated with the ref parameter (default value 1) as basis}
\item{likelihoods}{for each pedigree}
\item{likelihoodsPerSystem}{likelihoods for each locus and each pedigree}
}

\author{
Petter Mostad <mostad@chalmers.se> and Thore Egeland <Thore.Egeland@nmbu.com>
}

\examples{
#Example 1
#Data is available for "mother", "daughter" and "AF" for two loci (systems). 
#Three pedigrees are defined, with "mother" being the mother of "daughter" 
#in all cases. "AF" may be the father (ped1), unrelated (ped1) or
#uncle (ped3). The posterior probabilities for the pedigrees are calculated 
#and likelihoods are also given so that
#likelihood ratios can be computed. Compared to the windows version of Familias 2.0, 
#it is easy to plot pedigrees and define arbitrary priors for the three alternative pedigrees.
#The below implementation uses the R package kinship2 to define and plot pedigrees

persons <- c("mother", "daughter", "AF")
ped1 <- FamiliasPedigree(id = persons, dadid=c(NA, "AF", NA), momid=c(NA, "mother", NA), 
sex=c("female", "female", "male"))
ped2 <- FamiliasPedigree(id = c(persons, "TF"), dadid=c(NA, "TF", NA, NA), 
momid=c(NA, "mother", NA, NA), 
sex=c("female", "female", "male", "male"))
ped3 <- FamiliasPedigree(id = c(persons, "TF", "gf", "gm"), 
dadid = c(NA, "TF", "gf", "gf", NA, NA), 
momid = c(NA, "mother", "gm", "gm", NA, NA), 
sex = c("female", "female", "male", "male", "male", "female"))
par(mfrow=c(3,1))
plot(ped1);title("ped1, i.e., AF is father")
plot(ped2);title("ped2, i.e., AF is unrelated")
plot(ped3);title("ped3, i.e., AF is uncle")
par(mfrow = c(1,1))
mypedigrees <- list(isFather = ped1, unrelated = ped2, isUncle = ped3)
locus1 <- FamiliasLocus(frequencies=c(0.1, 0.2, 0.3, 0.4), 
allelenames = c("A", "B", "C", "D"), name = "locus1")
locus2 <- FamiliasLocus(c(0.2, 0.3, 0.5), c(17, 18, 19), "loc2", femaleMutationRate = 0.05)
myloci <- list(locus1, locus2)
datamatrix <- data.frame(locus1.1 = c("A", "A", "A"), locus1.2 =c ("B", "B", "C"), 
locus2.1=c(17, 19, 19), locus2.2=c(18, 18, 18))
rownames(datamatrix) <- persons
result <- FamiliasPosterior(mypedigrees, myloci, datamatrix,ref = 2)



#Example 2: Using FamiliasPedigree
persons <- c("person", "AF")
sex <- c("male", "male")
ped1 <- FamiliasPedigree(id = persons, dadid = c(NA, NA), momid = c(NA, NA), sex = sex)
ped2 <- FamiliasPedigree(id = persons, dadid = c("AF", NA), momid = c(NA, NA), sex = sex)
mypedigrees <- list(unrelated = ped1, isFather = ped2)
locus1 <- FamiliasLocus(c(0.1, 0.2, 0.3, 0.4), c("A", "B", "C", "D"), "locus1", 
   maleMutationModel = "Equal", maleMutationRate = 0.005)
locus2 <- FamiliasLocus(c(0.2, 0.3, 0.5), c(17, 18, 19), "locus2", 
   maleMutationModel = "Equal", maleMutationRate = 0.005) 
myloci <- list(locus1, locus2)
datamatrix <- data.frame(locus1.1 = c("A", "A"), locus1.2 = c("B", "B"), 
locus2.1 = c(17, 19), locus2.2 = c(18, 18))
rownames(datamatrix) <- persons
result <- FamiliasPosterior(mypedigrees, myloci, datamatrix)

#Example 3: User-specified mutation matrices
persons <- c("son", "mother", "AF")
sex <- c("male", "female", "male")
ped1 <- FamiliasPedigree(id=persons, dadid = c(NA, NA, NA), momid = c("mother", NA, NA), sex=sex)
ped2 <- FamiliasPedigree(id=persons, dadid = c("AF", NA, NA), momid = c("mother", NA, NA), sex=sex)
mypedigrees <- list(unrelated = ped1, isFather = ped2)
locus1 <- FamiliasLocus(c(0.1, 0.2, 0.3, 0.4), c("A", "B", "C", "D"), "locus1", 
   maleMutationModel = "Custom", maleMutationMatrix = matrix(
   c(0.99, 0.005, 0.003, 0.002, 
     0.004, 0.99, 0.004, 0.002, 
     0.002, 0.004, 0.99, 0.004, 
     0.002, 0.003, 0.005, 0.99), 
   4, 4, byrow = TRUE), femaleMutationModel = "Custom", femaleMutationMatrix = matrix(
   c(0.99, 0.005, 0.003, 0.002, 
     0.004, 0.99, 0.004, 0.002, 
     0.002, 0.004, 0.99, 0.004, 
     0.002, 0.003, 0.005, 0.99), 
   4, 4, byrow = TRUE))
datamatrix <- data.frame(locus1.1 = c("A", "A", "C"), locus1.2 = c("B", "A", "C"))
rownames(datamatrix) <- persons
result <- FamiliasPosterior(mypedigrees, locus1, datamatrix)

#Example 4: Adding an untyped ancestor affects the likelihood
#when subpopulation correction is used even if the mutation matrix is stationary

# ped1 is son by himself
ped1 <- FamiliasPedigree(id="son", 
                         dadid = c(NA), 
                         momid = c(NA), 
                         sex="male")

# ped2 describes son with parents
persons <- c("son", "mother", "AF")
sex <- c("male", "female", "male")
ped2 <- FamiliasPedigree(id=persons, 
                         dadid = c("AF", NA, NA), 
                         momid = c("mother", NA, NA), 
                         sex=sex)

# only the son is typed
datamatrix <- matrix(c("A","A"), ncol=2)
rownames(datamatrix) <- "son"

f <- setNames(c(0.3, 0.7), c("A", "B"))

locus1 <- FamiliasLocus(f, 
                        MutationModel = "Proportional", 
                        MutationRate = 1e-2)

# if Fst=0, then ped1 and ped2 are equivalent
pr_ped1_HW <- FamiliasPosterior(list(ped1), locus1, datamatrix)$likelihoods
pr_ped2_HW <- FamiliasPosterior(list(ped2), locus1, datamatrix)$likelihoods

# verify calculations by hand
stopifnot(all.equal(pr_ped1_HW, pr_ped2_HW))
stopifnot(all.equal(pr_ped1_HW, as.numeric(f["A"]^2)))
stopifnot(all.equal(pr_ped2_HW, as.numeric(f["A"]^2)))

# if Fst>0, then ped1 and ped2 are not equivalent
Fst <- 0.03

pr_ped1_Fst <- FamiliasPosterior(list(ped1), locus1, datamatrix, kinship = Fst)$likelihoods
pr_ped2_Fst <- FamiliasPosterior(list(ped2), locus1, datamatrix, kinship = Fst)$likelihoods

# pr_ped1_Fst and pr_ped2_Fst should not be equal: stop if equal
stopifnot(!isTRUE(all.equal(pr_ped1_Fst, pr_ped2_Fst)))

# compute pr_ped1_Fst by hand to verify the result
pr_ped1_Fst_manual <- as.numeric(f["A"]*Fst + f["A"]^2 * (1-Fst))
stopifnot(all.equal(pr_ped1_Fst, pr_ped1_Fst_manual))

# compute pr_ped2_Fst by hand to verify the result
# computing Pr(son=A,A|ped2) with subpopulation correction and mutations
# requires an explicit sum over the two parental alleles

pr_AA <- as.numeric(f["A"]*Fst + f["A"]^2 * (1-Fst))
pr_AB <- 2 * as.numeric(f["A"]*f["B"] * (1-Fst))
pr_BB <- as.numeric(f["B"]*Fst + f["B"]^2 * (1-Fst))

M <- locus1$maleMutationMatrix

pr_AA_given_AA <- M["A","A"]^2
pr_AA_given_AB <- M["A","A"] * M["B","A"]
pr_AA_given_BB <- M["B","A"]^2

pr_ped2_Fst_manual <- pr_AA*pr_AA_given_AA +pr_AB*pr_AA_given_AB + pr_BB*pr_AA_given_BB

stopifnot(all.equal(pr_ped2_Fst, pr_ped2_Fst_manual))


}