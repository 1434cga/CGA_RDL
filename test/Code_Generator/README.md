# How to run or test
- make init
- make
- make test

# get sub module when you clone git.
- git clone --recurse-submodules ssh://cheoljoo.lee@vgit.lge.com:29420/tiger/services/TIDL

# When you clone git only, you run the following command.
- git submodule init
- git submodule update 
    - Then you can show the file in CGA_RDL

# run it when you want to sync-up with recent remote repository.
- git submodule update --remote CGA_RDL


# reference
- git submodules : https://git-scm.com/book/en/v2/Git-Tools-Submodules



