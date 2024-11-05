
source ./setup_variables.sh
echo
echo Compiler: 
echo ${CC}
echo
echo Cleaning build folder..
mkdir -p ../build
(cd ../build && rm -Rfv *)
(cd ../build && rm -Rfv .cmake)
echo
echo Running cmake
(cd ../build && cmake ..)