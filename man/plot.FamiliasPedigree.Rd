\name{plot.FamiliasPedigree}
\alias{plot.FamiliasPedigree}
\title{Plotting function for FamiliasPedigree objects
}
\description{
The function piggybacks the plot function for pedigree objects from the kinship2 package to create a plotting function for FamiliasPedigree objects. 
Before conversion to a pedigree object, additional parents are added to the pedigree so that each person has either zero or two parents within the pedigree. 
}
\usage{
\method{plot}{FamiliasPedigree}(x, y, ...)
}
\arguments{
  \item{x}{
An object of class FamiliasPedigree
}
  \item{y}{
Not used in this printing function
}
  \item{\dots}{
Other arguments are transferred to the plot.pedigree function of the kinship2 package.  
}
}
\details{
%%  ~~ If necessary, more details than the description above ~~
Graphical parameters used in the plot.pedigree function may be input via the plot.FamiliasPedigree function. 
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
%%  ~~who you are~~
Petter Mostad mostad@chalmers.se 
}
\seealso{ 'pedtools', \url{https://github.com/magnusdv/pedtools} }
\examples{
ped <- FamiliasPedigree(id = c("child", "AF"), momid = c(NA, NA), 
       dadid = c("AF", NA), sex = c("male", "male"))
plot(ped)
dev.new()
plot(ped, symbolsize = 2, cex = 2, family = "mono")
}

