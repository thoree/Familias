\name{NorwegianFrequencies}
\alias{NorwegianFrequencies}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{
A list of markers with allele names and frequencies 
}
\description{
The information represents allele frequencies for a range of different markers based on 
a data base in Norway. 
}
\usage{
data(NorwegianFrequencies)
}
%- maybe also 'usage' for other objects documented here.
\format{
A list with named components, with names corresponding to the names of the loci. 
Each component is a vector of frequencies, with the names attribute of the vector 
equal to the names of the alleles. 
}
\source{
Norwegian Institute of Public Health, Department of Family Genetics: 
\cite{Dupuy et al: Frequency data for 35 autosomal STR markers in a Norwegian, an East African, 
and East Asian and Middle Asian population and simulation of adequate database size. Forensic Science 
International: Genetics Supplement Series, Volume 4, issue 1, 2013}
}
\examples{
data(NorwegianFrequencies)

#Displaying the Norwegian frequencies of the 
NorwegianFrequencies$TPOX

#Generating a FamiliasLocus with these frequencies
FamiliasLocus(NorwegianFrequencies$TPOX)

#Including a non-zero male mutation rate
FamiliasLocus(NorwegianFrequencies$TPOX, maleMutationRate = 0.005)

#Listing the names of available markers
names(NorwegianFrequencies)
}