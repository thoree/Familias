sudo snap docker  # if you dont have docker installed
sudo docker run -it --rm registry.fedoraproject.org/fedora:40 bash # This is the fedora installer
dnf install -y R gcc-c++ git && 
git clone https://github.com/cran/Familias.git && cd Familias && R CMD INSTALL --preclean .


rhub::rhub_doctor()
.Lastdevtools::check_win_devel()
devtools::check()
rhub::check_for_cran() 

install.packages("rhub")
library(rhub)
check(platform = "windows-x86_64-devel")        # Similar to check_win_devel()
check(platform = "ubuntu-gcc-release")          # Linux GCC compiler
check(platform = "fedora-clang-devel") 

rhub::platforms()

usethis::use_github_action_check_standard()