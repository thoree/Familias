\name{FamiliasPrior}
\alias{FamiliasPrior}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{
Calculates a prior distribution for a list of pedigrees
}
\description{
By default the same prior probability is assigned to all pedigrees in the list, but this can be adjusted with the 
function parameters. 
It is computed which persons are common to all the pedigrees listed, and they are handled in a special way: 
It is with relation to these persons that the number of generations and other parameters are computed. Also, the function will 
search for and remove pedigrees that are "equivalent" in terms of representing the relationship between these core persons. 
So if another pedigree is added, with all new persons, the function will return with an error message. 
}
\usage{
FamiliasPrior(pedigrees, generationsParameter = 1, 
inbreedingParameter = 1, partnerParameter = 1, maxGenerations)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{pedigrees}{
A list of objects of class either 'pedigree' or 'FamiliasPedigree'.
}
  \item{generationsParameter}{
Non-negative real. A value of 1 
indicates no influence of the parameter. A value above 1 (below 1) increases (decreases) the prior probability 
for pedigrees with many generations.
}
  \item{inbreedingParameter}{
Non-negative real. A pedigree is inbred if parents are related within the pedigree. 
If 0, all inbred pedigrees are assigned a prior probability 0. A value of 1 
indicates no influence of inbreeding. A value above 1 (below 1) increases (decreases) the prior for inbred pedigrees.
}
  \item{partnerParameter}{
Non-negative real (previously referred to as promiscuity parameter). If 0, all pedigrees
where parents have children by different partners, are assigned prior 0. A value of 1 
indicates no influence of the parameter. A value above 1 (below 1) increases (decreases) the prior for partners having children by different partners. 
}
  \item{maxGenerations}{
Integer giving the maximum number of generations; pedigrees with more generations than this are assigned a zero prior probability. 
}
}
\details{
See https://familias.name/manual.pdf for a complete description of the parametric models implemented.
}
\value{
The prior, i.e., a vector of real numbers summing to 1.
}

\author{
Petter Mostad <mostad@chalmers.se> and Thore Egeland <Thore.Egeland@nmbu.com>
}

\examples{
persons <- c("mother", "daughter", "AF")
ped1 <- FamiliasPedigree(id = persons, dadid = c(NA, "AF", NA), 
                         momid = c(NA, "mother", NA),
                         sex = c("female", "female", "male"))
ped2 <- FamiliasPedigree(id = c(persons, "TF"), dadid = c(NA, "TF", NA, NA), 
momid <- c(NA, "mother", NA, NA), 
sex <- c("female", "female", "male", "male"))
ped3 <- FamiliasPedigree(id = c(persons, "TF", "gf", "gm"), 
dadid  <-  c(NA, "TF", "gf", "gf", NA, NA), 
momid <- c(NA, "mother", "gm", "gm", NA, NA), 
sex <- c("female", "female", "male", "male", "male", "female"))
mypedigrees <- list(isFather = ped1, unrelated = ped2, isUncle = ped3)
FamiliasPrior(mypedigrees)
granddad <- FamiliasPedigree(id = c(persons, "TF", "gm"), 
                             dadid = c(NA, "TF", NA, "AF", NA), 
momid <- c(NA, "mother", NA, "gm", NA), 
           sex = c("female", "female", "male", "male", "female"))
FamiliasPrior(c(mypedigrees, list(granddad)))
FamiliasPrior(c(mypedigrees, list(granddad)), maxGenerations = 1)
}