
. ./setup_variables.sh
echo make -j$(nproc)
(cd ../build && make all -j$(nproc))