
# source ./setup_variables.sh
echo
echo Compiler: 
echo ${CC}
echo
echo Cleaning build folder..
mkdir -p ../build/tests
(cd ../build/tests && rm -Rfv *)
echo
echo Running cmake
(cd ../build/tests && cmake ../../test)