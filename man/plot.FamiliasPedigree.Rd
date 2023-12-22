\name{plot.FamiliasPedigree}
\alias{plot.FamiliasPedigree}
\title{Plotting function for FamiliasPedigree objects
}
\description{
The function piggybacks the plot function for pedigree objects from the kinship2
package to create a plotting function for FamiliasPedigree objects. Before 
conversion to a pedigree object, additional parents are added to the pedigree 
so that each person has either zero or two parents within the pedigree. 
}
\usage{
\method{plot}{FamiliasPedigree}(x, y, ...)
}
\arguments{
  \item{x}{
An object of class FamiliasPedigree.
}
  \item{y}{
Not used in this printing function.
}
  \item{\dots}{
Other arguments transferred to `kinship2::plot.pedigree()`.  
}
}
\details{
%%  ~~ If necessary, more details than the description above ~~
Graphical parameters used in `kinship2::plot.pedigree()` may be input via 
`plot.FamiliasPedigree()`. 
}
\value{
%%  ~Describe the value returned
%%  If it is a LIST, use
%%  \item{comp1 }{Description of 'comp1'}
%%  \item{comp2 }{Description of 'comp2'}
%% ...
A plot is produced. 
}
\author{
Petter Mostad mostad@chalmers.se 
}

\examples{
ped <- FamiliasPedigree(id = c("child", "AF"), 
                        momid = c(NA, NA), 
                        dadid = c("AF", NA), 
                        sex = c("male", "male"))
plot(ped)
dev.new()
plot(ped, symbolsize = 2, cex = 2, family = "mono")
}

